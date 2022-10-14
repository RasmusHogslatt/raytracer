#pragma once

#include <integrators/MonteCarloIntegrator.h>
#include <iostream>
#include <random>
#include <string>
#include <memory>
#include <imgui.h>
#include <glm/geometric.hpp>
#include <cmath>
#include <glm/gtx/norm.hpp>

#include <thread>

std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(0, 1);

MonteCarloIntegrator::MonteCarloIntegrator(Scene* scene) : Integrator(scene), samplerPerPixel_{ 1 }, bias_{ 0.0001f }, lightSamples_{ 1 }, progress_{ 0 }, indirectSampleOption_{ 1 } {}

void MonteCarloIntegrator::Integrate(Texture& renderTexture) {
	glm::ivec2 resolution = scene_->cameras_[scene_->activeCamera_]->resolution_;

	Ray ray;
	glm::vec3 color;
	glm::vec2 samplePosition = glm::vec2(0);
	int currentSample = 0;

	for (int y = 0; y < resolution.y; ++y) {
		for (int x = 0; x < resolution.x; ++x) {
			glm::vec3 pixel = glm::vec3(0);
			color = glm::vec3(0);
			for (currentSample = 0; currentSample < samplerPerPixel_; ++currentSample) {
				samplePosition = scene_->samplers_[scene_->activeSampler_]->getSamplePosition(x, y, *scene_->cameras_[scene_->activeCamera_], currentSample);
				ray = scene_->cameras_[scene_->activeCamera_]->generateRay(samplePosition);
				ray.parent_ = nullptr;
				Ray pr = Ray();
				ray.parent_ = std::make_shared<Ray>(pr);
				color += traceRay(ray);
			}
			color /= static_cast<float>(samplerPerPixel_);
			renderTexture.setPixelColor(x, y, ImVec4(color.r, color.g, color.b, 1.0f));
		}
	}
	renderTexture.updateTextureData();
}
//set r.radiance
//set outgoing ray.importance
// For each ray calculate importance, then return color scaled by incoming rays importance
glm::vec3 MonteCarloIntegrator::traceRay(Ray& r) {
	// Get closest intersected object
	glm::vec3 color = glm::vec3(0);
	float objectDistance = std::numeric_limits<float>::max();
	int objectIndex = getNearestIntersectionIndex(r, objectDistance, 0);

	// Get closest intersected light
	float lightDistance = std::numeric_limits<float>::max();
	int lightIndex = getNearestIntersectionIndex(r, lightDistance, 1);

	// Light hit before object --> Terminate
	if (lightIndex != -1 && lightDistance < objectDistance) {
		r.radiance_ += glm::vec3(scene_->lights_[lightIndex]->flux_ / scene_->lights_[lightIndex]->shape_->getArea());
		r.lightIndex_ = lightIndex;
		return glm::vec3(1);
	}
	// Final hit is lambertian
	else {
		// Intersection found
		if (objectIndex != -1) {
			r.material_ = scene_->primitives_[objectIndex]->material_;
			r.end_ = r.origin_ + r.direction_ * objectDistance;
			r.intersectionNormal_ = scene_->primitives_[objectIndex]->shape_->getNormal(r.end_);
			if (r.intersectionNormal_ != glm::vec3(-1.0f, 1.0f, 0.0f)) {
				r.orthoToNormal_ = glm::normalize(glm::vec3(r.intersectionNormal_.z, r.intersectionNormal_.z, -r.intersectionNormal_.x - r.intersectionNormal_.y));
			}
			else {
				r.orthoToNormal_ = glm::normalize(glm::vec3(-r.intersectionNormal_.y - r.intersectionNormal_.z, r.intersectionNormal_.x, r.intersectionNormal_.x));
			}

			// Check inside or outside object
			if (glm::dot(r.direction_, r.intersectionNormal_) < 0.0f) {
				r.inside_ = false;
			}
			else {
				r.inside_ = true;
			}

			switch (r.material_->id_) {
			case 0: {
				if (!r.inside_) {

					color += directLight(r);

					float pdf = 1.0f;
					glm::vec3 sampleDirection = glm::vec3(0);

					float phi = distribution(generator);
					float theta = distribution(generator);
					if (russianRoulette(phi)) {
						switch (indirectSampleOption_) {
						case 0:
							pdf = 1.0f / (2.0f * 3.14f);

							sampleDirection = uniformSampleHemisphere(r, phi, theta);
							break;
						case 1:
							/*phi *= 2.0f * 3.14f;
							theta = acos(sqrt(theta));*/
							sampleDirection = cosineWeightedSampleHemisphere(r, phi, theta);
							pdf = glm::dot(r.intersectionNormal_, sampleDirection) / 3.14f;
							break;
						}
						Ray indirectRay = Ray(r.end_, sampleDirection, std::make_shared<Ray>(r));
						r.child_ = std::make_shared<Ray>(indirectRay);

						// Trace indirect ray and adjust for incoming angle and propbability of specific ray
						indirectRay.importance_ = r.importance_ * pdf; // eq. 5 w2 = pi / pdf * fr * w1

						color += traceRay(indirectRay) * indirectRay.importance_ / r.importance_; // -> 0 elementwise
					}
				}
				break;
			}
			case 1: {
				glm::vec3 intersectionOffset = r.end_ + r.intersectionNormal_ * bias_;
				glm::vec3 direction = glm::reflect(r.direction_, r.intersectionNormal_);
				Ray reflectedRay(intersectionOffset, direction, std::make_shared<Ray>(r));
				//reflectedRay.radiance_ = r.radiance_;
				reflectedRay.importance_ = r.importance_;
				color += traceRay(reflectedRay);

				break;
			}
			case 2: {
				// Calculate new ray origins with offset + ratio of ior
				float sign = 1.0f;
				float n1 = globalIOR_;
				float n2 = r.material_->ior_;
				if (r.inside_) {
					sign = -1.0f;
					std::swap(n1, n2);
				}

				glm::vec3 reflectedOrigin = r.end_ + r.intersectionNormal_ * bias_ * sign;
				glm::vec3 refractedOrigin = r.end_ - r.intersectionNormal_ * bias_ * sign;

				// Reflective coefficient
				float reflectiveCoefficient = schlickApproximation(r.direction_, r.intersectionNormal_, n1, n2);

				// If true, send refractive ray and no reflected ray (and check TIR)
				bool sendRefractiveRay = russianRoulette(reflectiveCoefficient);

				// Calculate refracted ray
				bool TIR = false; // Updated by getRefractionDirection
				glm::vec3 refractedDirection = getRefractionDirection(r.direction_, r.intersectionNormal_, n1, n2, TIR);

				Ray reflectedRay(refractedOrigin, refractedDirection, std::make_shared<Ray>(r));
				r.child_ = std::make_shared<Ray>(reflectedRay);
				reflectedRay.importance_ = r.importance_; // W2 = W1 for perfect reflection/refraction with russian roulette

				// Refraction
				if (!TIR && sendRefractiveRay) {
					Ray refractedRay(refractedOrigin, refractedDirection, std::make_shared<Ray>(r));
					if (r.inside_) {
						reflectedRay.inside_ = false;
					}
					else {
						reflectedRay.inside_ = true;
					}
				}
				// Reflection
				else if (!sendRefractiveRay) {

					// Calculate reflected ray
					glm::vec3 reflectedDirection = glm::reflect(r.direction_, r.intersectionNormal_);
					reflectedRay.origin_ = reflectedOrigin;
					reflectedRay.direction_ = reflectedDirection;
					if (r.inside_) {
						reflectedRay.inside_ = true;
					}
					else {
						reflectedRay.inside_ = false;
					}
				}
				color += traceRay(reflectedRay);
				break;
			}
			}
		}
	}
	return glm::clamp(color, 0.0f, 1.0f);
}

int MonteCarloIntegrator::getNearestIntersectionIndex(Ray& r, float& depth, int type)
{
	float distance = std::numeric_limits<float>::max();
	float currentDistance;
	int index = -1;
	if (type == 0) { // Primitives
		for (int i = 0; i < scene_->primitives_.size(); ++i) {
			if (scene_->primitives_[i]->shape_->intersect(r, currentDistance)) {
				if (currentDistance < distance) {
					distance = currentDistance;
					index = i;
					depth = distance;

				}
			}
		}
		return index;
	}
	else { // Lights
		for (int i = 0; i < scene_->lights_.size(); ++i) {
			if (scene_->lights_[i]->shape_->intersect(r, currentDistance)) {
				if (currentDistance < distance) {
					distance = currentDistance;
					index = i;
					depth = distance;
				}
			}
		}
		return index;
	}
}

// Generates a direction vector around normal with variance given by r1 and r2
glm::vec3 MonteCarloIntegrator::uniformSampleHemisphere(const Ray& ray, const float& phi, const float& theta)
{
	glm::vec3 randomDirection = glm::normalize(glm::rotate(
		ray.orthoToNormal_,
		phi,
		ray.intersectionNormal_));
	randomDirection = glm::normalize(glm::rotate(
		ray.intersectionNormal_,
		theta,
		randomDirection));
	return glm::normalize(randomDirection);
}

glm::vec3 MonteCarloIntegrator::cosineWeightedSampleHemisphere(const Ray& ray, const float& phi, const float& theta)
{
	glm::vec3 randomDirection = glm::normalize(glm::rotate(
		ray.orthoToNormal_,
		phi,
		ray.intersectionNormal_));
	randomDirection = glm::normalize(glm::rotate(
		ray.intersectionNormal_,
		theta,
		randomDirection));
	return glm::normalize(randomDirection);
}

// Calculates direct lighting given normal at intersected object and ray which holds material properties and intersectionPoint = ray.end_
glm::vec3 MonteCarloIntegrator::directLight(Ray& ray)
{
	glm::vec3 color = glm::vec3(0);
	for (int i = 0; i < scene_->lights_.size(); ++i) {
		Light* light = scene_->lights_[i];

		// Get random point on light
		float r1 = distribution(generator);
		float r2 = distribution(generator);
		glm::vec3 pointOnLightHemisphere = light->shape_->getPointOnSurface(glm::normalize(ray.end_ - light->shape_->getPosition()), r1, r2);
		glm::vec3 lightToSurface = pointOnLightHemisphere - ray.end_;

		Ray lightRay = Ray(ray.end_, glm::normalize(lightToSurface));
		float lightToObjectDistance = glm::distance(pointOnLightHemisphere, lightRay.origin_);

		float x = 0.0f;
		float visibility = 1.0f;
		if (light->shape_->intersect(lightRay, x)) {
			// In shadow?
			float lightDistance = glm::distance(pointOnLightHemisphere, lightRay.origin_);
			if (getNearestIntersectionIndex(lightRay, x, 0) != -1) {
				if (x > lightDistance) {
					visibility = 1.0f;
				}
				else {
					visibility = 0.0f;
				}
			}

			float cosXi = glm::dot(ray.intersectionNormal_, pointOnLightHemisphere - ray.end_);
			float cosYi = glm::dot(-light->shape_->getNormal(pointOnLightHemisphere), lightToSurface);
			float radiosity = light->flux_ / light->shape_->getArea();
			color += ray.material_->color_ * ray.material_->getFr() * visibility * radiosity * cosXi * cosYi / (lightToObjectDistance * lightToObjectDistance);
		}
	}
	return color;
}

bool MonteCarloIntegrator::russianRoulette(float probability)
{
	return distribution(generator) >= probability;
}

void MonteCarloIntegrator::GUI() {

	ImGui::Text("Monte Carlo Integrator");

	ImGui::NewLine();
	ImGui::Text("GENERAL RENDER SETTINGS");
	ImGui::SliderFloat("BIAS", &bias_, 1e-4f, 1e-1f, "%.5f");
	ImGui::SliderInt("Samples per pixel", &samplerPerPixel_, 1, 100);
	Integrator::GUI();

	ImGui::NewLine();
	ImGui::Text("INDIRECT LIGHT TRANSPORT");
	ImGui::SliderInt("Mode : ", &indirectSampleOption_, 0, 1);
	ImGui::SameLine();
	switch (indirectSampleOption_) {
	case 0:
		ImGui::Text("Uniform");
		break;
	case 1:
		ImGui::Text("Cosine weighted");
		break;
	}

	ImGui::NewLine();
	ImGui::Text("DIRECT LIGHT TRANSPORT");
	ImGui::SliderInt("Samples per light", &lightSamples_, 0, 100);
}