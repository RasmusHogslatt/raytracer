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

std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(0, 1);

MonteCarloIntegrator::MonteCarloIntegrator(Scene* scene) : Integrator(scene), samplerPerPixel_{ 1 }, bias_{ 0.001f }, lightSamples_{ 1 }, progress_{ 0 }, indirectSampleOption_{ 1 } {}

void MonteCarloIntegrator::Integrate(Texture& renderTexture) {
	glm::ivec2 resolution = scene_->cameras_[scene_->activeCamera_]->resolution_;

	Ray ray;
	glm::vec3 color;
	glm::vec2 samplePosition = glm::vec2(0);
	int currentSample = 0;

	for (int y = 0; y < resolution.y; ++y) {
		for (int x = 0; x < resolution.x; ++x) {

			progress_++;
			//ImGui::ProgressBar(static_cast<float>(progress_) / static_cast<float>(1920 * 1080), ImVec2(0, 0), "s");
			color = glm::vec3(0);
			for (currentSample = 0; currentSample < samplerPerPixel_; ++currentSample) {
				samplePosition = scene_->samplers_[scene_->activeSampler_]->getSamplePosition(x, y, *scene_->cameras_[scene_->activeCamera_], currentSample);
				ray = scene_->cameras_[scene_->activeCamera_]->generateRay(samplePosition);
				ray.parent_ = nullptr;
				color += traceRay(ray);
			}
			//color = traverseList(std::make_shared<Ray>(ray));
			color /= static_cast<float>(samplerPerPixel_);
			renderTexture.setPixelColor(x, y, ImVec4(color.r, color.g, color.b, 1.0f));
		}
	}
	renderTexture.updateTextureData();
}

glm::vec3 MonteCarloIntegrator::traceRay(Ray& r) {
	glm::vec3 finalColor = glm::vec3(0);
	glm::vec3 directColor = glm::vec3(0);
	glm::vec3 indirectColor = glm::vec3(0);
	glm::vec3 reflectedColor = glm::vec3(0);
	glm::vec3 refractedColor = glm::vec3(0);

	// Get closest intersected object
	float objectDistance = std::numeric_limits<float>::max();
	int objectIndex = getNearestIntersectionIndex(r, objectDistance, 0);

	float lightDistance = std::numeric_limits<float>::max();
	int lightIndex = getNearestIntersectionIndex(r, lightDistance, 1);

	// Final hit is light
	if (lightIndex != -1 && lightDistance < objectDistance) {
		r.importance_ = glm::vec3(scene_->lights_[lightIndex]->getRadiosity());
		finalColor = glm::vec3(1);
	}
	// Final hit is lambertian
	else {
		// Intersection found
		if (objectIndex != -1) {
			r.material_ = scene_->primitives_[objectIndex]->material_;
			r.end_ = r.origin_ + r.direction_ * objectDistance;
			r.intersectionNormal_ = scene_->primitives_[objectIndex]->shape_->getNormal(r.end_);

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
					directColor = directLight(r);
					r.importance_ += directColor;//?
					indirectColor = indirectLight(r);
					finalColor += (directColor / 3.14f + indirectColor);
				}
				break;
			}
			case 1: {
				glm::vec3 intersectionOffset = r.end_ + r.intersectionNormal_ * bias_;
				glm::vec3 direction = glm::reflect(r.direction_, r.intersectionNormal_);
				Ray reflectedRay(intersectionOffset, direction, std::make_shared<Ray>(r));
				reflectedRay.importance_ = r.importance_;
				reflectedColor += traceRay(reflectedRay);
				finalColor += reflectedColor;
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
				else {

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
				if (!TIR && sendRefractiveRay) {
					Ray refractedRay(refractedOrigin, refractedDirection, std::make_shared<Ray>(r));
					refractedRay.importance_ = r.importance_;
					if (r.inside_) {
						refractedRay.inside_ = false;
					}
					else {
						refractedRay.inside_ = true;
					}
					r.child_ = std::make_shared<Ray>(refractedRay);
					refractedColor += traceRay(refractedRay) * (1.0f - reflectiveCoefficient);
				}
				else if (!sendRefractiveRay) {

					// Calculate reflected ray
					glm::vec3 reflectedDirection = glm::reflect(r.direction_, r.intersectionNormal_);
					Ray reflectedRay(reflectedOrigin, reflectedDirection, std::make_shared<Ray>(r));
					r.child_ = std::make_shared<Ray>(reflectedRay);
					reflectedRay.importance_ = r.importance_;
					if (r.inside_) {
						reflectedRay.inside_ = true;
					}
					else {
						reflectedRay.inside_ = false;
					}
					reflectedColor += traceRay(reflectedRay) * reflectiveCoefficient;
				}


				// Add colors
				finalColor += refractedColor + reflectedColor;
				break;
			}
			default:
				break;
			}
		}
		return glm::clamp(finalColor, 0.0f, 1.0f);
	}
	return glm::vec3(0);
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
glm::vec3 MonteCarloIntegrator::uniformSampleHemisphere(const Ray& ray, const float& r1, const float& r2)
{
	float inclination = glm::acos(r1);
	float azimuth = 2.0f * 3.14f * r2;
	glm::vec3 randomDirection = ray.intersectionNormal_;
	randomDirection = glm::normalize(glm::rotate(
		randomDirection,
		inclination,
		glm::vec3(0, 1, 0)));
	randomDirection = glm::normalize(glm::rotate(
		randomDirection,
		azimuth,
		glm::vec3(1, 0, 0)));
	return glm::normalize(randomDirection);
}

glm::vec3 MonteCarloIntegrator::cosineWeightedSampleHemisphere(const Ray& ray, const float& r1, const float& r2)
{
	float inclination = glm::acos(std::sqrt(r1));
	float azimuth = 2.0f * 3.14f * r2;
	glm::vec3 randomDirection = ray.intersectionNormal_;

	randomDirection = glm::normalize(glm::rotate(
		randomDirection,
		inclination,
		glm::vec3(0, 1, 0)));
	randomDirection = glm::normalize(glm::rotate(
		randomDirection,
		azimuth,
		glm::vec3(1, 0, 0)));
	return glm::normalize(randomDirection);
}

// Calculates direct lighting given normal at intersected object and ray which holds material properties and intersectionPoint = ray.end_
glm::vec3 MonteCarloIntegrator::directLight(Ray& ray)
{
	glm::vec3 color = glm::vec3(0);
	for (int i = 0; i < scene_->lights_.size(); ++i) {
		Light* light = scene_->lights_[i];
		for (int lightSamples = 0; lightSamples < lightSamples_; ++lightSamples) {

			// Get random point on light
			float r1 = distribution(generator);
			float r2 = distribution(generator);
			glm::vec3 pointOnLightHemisphere = light->shape_->getPointOnSurface(glm::normalize(ray.end_ - light->shape_->getPosition()), r1, r2);

			glm::vec3 lightDirection = glm::normalize(pointOnLightHemisphere - ray.end_);
			Ray lightRay = Ray(ray.end_, lightDirection);
			float x;
			float shadow = 1.0f;
			if (light->shape_->intersect(lightRay, x)) {
				// In shadow?
				if (getNearestIntersectionIndex(lightRay, x, 0) != -1) {
					if (x > glm::distance(pointOnLightHemisphere, ray.origin_)) {
						shadow = 1.0f;
					}
					else {
						shadow = 0.0f;
					}
				}
				color += ray.material_->color_ * std::max(glm::dot(lightDirection, ray.intersectionNormal_), 0.0f) * shadow * light->getRadiosity() * static_cast<float>(lightSamples_);
			}
		}
	}
	color /= static_cast<float>(lightSamples_);
	ray.importance_ = color;
	return color;
}

glm::vec3 MonteCarloIntegrator::indirectLight(Ray& ray)
{
	glm::vec3 color = glm::vec3(0);
	float pdf = 1.0f;
	glm::vec3 sampleDirection = glm::vec3(0);
	float r1 = distribution(generator);
	float r2 = distribution(generator);
	if (!russianRoulette(r2)) {
		return glm::vec3(0);
	}
	switch (indirectSampleOption_) {
	case 0:
		pdf = 1.0f / (2.0f * 3.14f);
		sampleDirection = uniformSampleHemisphere(ray, r1, r2);
		break;
	case 1:
		pdf = pdf = glm::dot(ray.intersectionNormal_, ray.direction_) / 3.14f;
		sampleDirection = cosineWeightedSampleHemisphere(ray, r1, r2);
		break;
	}
	Ray indirectRay = Ray(ray.end_, sampleDirection, std::make_shared<Ray>(ray));
	ray.child_ = std::make_shared<Ray>(indirectRay);
	// Trace indirect ray and adjust for incoming angle and propbability of specific ray
	color += traceRay(indirectRay) * std::max(glm::dot(indirectRay.direction_, ray.intersectionNormal_), 0.0f) / pdf;

	indirectRay.importance_ = ray.importance_ * 3.14f / pdf * std::max(glm::dot(indirectRay.direction_, ray.intersectionNormal_), 0.0f);
	return color;
}

glm::vec3 MonteCarloIntegrator::traverseList(std::shared_ptr<Ray> r)
{
	glm::vec3 color = glm::vec3(0);
	if (r->material_ == nullptr) {
		return glm::vec3(0);
	}
	switch (r->material_->id_) {
	case 0:
		color += directLight(*r) / 3.14f;
		break;
	case 1: case 2:

		if (r->child_ != nullptr) {
			traverseList(r->child_);
		}
		break;
	}

	return glm::clamp(color, 0.0f, 1.0f);
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