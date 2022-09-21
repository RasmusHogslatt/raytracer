#pragma once

#include <integrators/MonteCarloIntegrator.h>
#include <iostream>
#include <random>
#include <string>
#include <memory>
#include <imgui.h>
#include <glm/geometric.hpp>
#include <cmath>

std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(0, 1);

MonteCarloIntegrator::MonteCarloIntegrator(Scene* scene) : Integrator(scene), maxRecursiveDepth_{ 1 }, samplerPerPixel_{ 1 }, bias_{ 0.1f }, indirectLightSamplesOnHemisphere_{ 1 }, lightSamples_{ 1 }, progress_{ 0 } {}

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
				ray.depth_ = 0;
				ray.parent_ = nullptr;
				color = traceRay(ray);
			}
			//color = traverseTree(ray);
			renderTexture.setPixelColor(x, y, ImVec4(color.r, color.g, color.b, 1.0f));
		}
	}
	renderTexture.updateTextureData();
}

glm::vec3 MonteCarloIntegrator::traceRay(Ray& r) {
	// Terminate recursion
	if (r.depth_ > maxRecursiveDepth_) {
		r.reflected_ = nullptr;
		r.refracted_ = nullptr;
		return glm::vec3(0);
	}

	glm::vec3 finalColor = glm::vec3(0);
	glm::vec3 directColor = glm::vec3(0);
	glm::vec3 indirectColor = glm::vec3(0);
	glm::vec3 reflectedColor = glm::vec3(0);
	glm::vec3 refractedColor = glm::vec3(0);

	// Get closest intersected object
	float distance = 0.0f;
	int objectIndex = getNearestIntersectionIndex(r, distance, 0);

	// Intersection found
	if (objectIndex != -1) {
		r.material_ = scene_->primitives_[objectIndex]->material_;
		r.end_ = r.origin_ + r.direction_ * distance;
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
			directColor += directLight(r);
			//indirectColor += indirectLight(r);
			finalColor += (directColor / 3.14f + 2.0f * indirectColor);
			break;
		}
		case 1: {
			if (r.depth_ + 1 <= maxRecursiveDepth_) {
				glm::vec3 intersectionOffset = r.end_ + r.intersectionNormal_ * bias_;
				glm::vec3 direction = glm::reflect(r.direction_, r.intersectionNormal_);
				Ray reflectedRay(intersectionOffset, direction, r.depth_ + 1, std::make_shared<Ray>(r));
				reflectedColor += traceRay(reflectedRay);
				finalColor += reflectedColor;
			}
			break;
		}
		case 2: {
			if (r.depth_ + 1 <= maxRecursiveDepth_) {
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

				// Calculate refracted ray
				bool TIR = false; // Updated by getRefractionDirection
				glm::vec3 refractedDirection = getRefractionDirection(r.direction_, r.intersectionNormal_, n1, n2, TIR);
				if (!TIR) {
					Ray refractedRay(refractedOrigin, refractedDirection, r.depth_ + 1, std::make_shared<Ray>(r));
					if (r.inside_) {
						refractedRay.inside_ = false;
					}
					else {
						refractedRay.inside_ = true;
					}
					r.refracted_ = std::make_shared<Ray>(refractedRay);
					refractedColor += traceRay(refractedRay) * (1.0f - reflectiveCoefficient);
				}

				// Calculate reflected ray
				glm::vec3 reflectedDirection = glm::reflect(r.direction_, r.intersectionNormal_);
				Ray reflectedRay(reflectedOrigin, reflectedDirection, r.depth_ + 1, std::make_shared<Ray>(r));
				r.reflected_ = std::make_shared<Ray>(reflectedRay);
				if (r.inside_) {
					reflectedRay.inside_ = true;
				}
				else {
					reflectedRay.inside_ = false;
				}
				reflectedColor += traceRay(reflectedRay) * reflectiveCoefficient;

				// Add colors
				finalColor += refractedColor + reflectedColor;
			}
			break;
		}
		default:
			break;
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
glm::vec3 MonteCarloIntegrator::uniformSampleHemisphere(const glm::vec3& normal, const float& r1, const float& r2)
{
	float inclination = glm::acos(1 - 2 * r1);
	float azimuth = 2.0f * 3.14f * r2;
	glm::vec3 randomDirection = normal;

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
glm::vec3 MonteCarloIntegrator::directLight(const Ray& ray)
{
	glm::vec3 color = glm::vec3(0);
	for (int i = 0; i < scene_->lights_.size(); ++i) {
		Light* light = scene_->lights_[i];
		for (int lightSamples = 0; lightSamples < lightSamples_; ++lightSamples) {

			// Get random numbers
			float r1 = distribution(generator);
			float r2 = distribution(generator);
			glm::vec3 pointOnLightHemisphere = light->shape_->getPointOnSurface(glm::normalize(ray.end_ - light->shape_->getPosition()), r1, r2);

			glm::vec3 lightDirection = glm::normalize(light->shape_->getPosition() - ray.end_);
			Ray lightRay = Ray(ray.end_, lightDirection);
			float x;
			if (light->shape_->intersect(lightRay, x)) {
				color += ray.material_->color_ * std::max(glm::dot(lightDirection, ray.intersectionNormal_), 0.0f) / static_cast<float>(lightSamples_);
			}
		}
	}
	return color;
}

glm::vec3 MonteCarloIntegrator::indirectLight(const Ray& ray)
{
	glm::vec3 color = glm::vec3(0);
	float pdf = 1.0f / (2.0f * 3.14f); // PDF = probability to sample specific direction in solid angle (current is hardcoded for full hemisphere)
	for (int n = 0; n < indirectLightSamplesOnHemisphere_; ++n) {
		float r1 = distribution(generator);
		float r2 = distribution(generator);

		glm::vec3 sampleDirection = uniformSampleHemisphere(ray.intersectionNormal_, r1, r2);
		Ray indirectRay = Ray(ray.end_, sampleDirection, ray.depth_ + 1);

		// Trace indirect ray and adjust for incoming angle and propbability of specific ray
		color += traceRay(indirectRay) * std::max(glm::dot(indirectRay.direction_, ray.intersectionNormal_), 0.0f) / pdf;
	}
	color /= static_cast<float>(indirectLightSamplesOnHemisphere_);
	return color;
}

glm::vec3 MonteCarloIntegrator::traverseTree(const Ray& root)
{
	glm::vec3 color = glm::vec3(0);


	color += directLight(root) / 3.14f;
	if (root.reflected_ != nullptr) {
		traverseTree(*root.reflected_);
	}
	if (root.refracted_ != nullptr) {
		traverseTree(*root.refracted_);
	}


	return glm::clamp(color, 0.0f, 1.0f);
}

void MonteCarloIntegrator::GUI() {

	ImGui::Text("Monte Carlo Integrator");
	ImGui::SliderInt("Max recursive depth", &maxRecursiveDepth_, 0, 10);
	ImGui::SliderInt("Indirect rays on hemisphere", &indirectLightSamplesOnHemisphere_, 0, 100);
	ImGui::SliderInt("Samples per light", &lightSamples_, 0, 100);
	ImGui::SliderFloat("BIAS", &bias_, 1e-4f, 1e0f, "%.5f");
	Integrator::GUI();
}