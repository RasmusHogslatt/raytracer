#pragma once

#include <integrators/MonteCarloIntegrator.h>
#include <iostream>
#include <random>
#include <string>
#include <memory>

std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(0, 1);

MonteCarloIntegrator::MonteCarloIntegrator(Scene* scene) : Integrator(scene), maxRecursiveDepth_{ 1 }, samplerPerPixel_{ 1 }, bias_{ 0.001f }, indirectLightSamplesOnHemisphere_{ 5 }, lightSamples_{ 5 } {}

void MonteCarloIntegrator::Integrate(Texture& renderTexture) {
	glm::ivec2 resolution = scene_->cameras_[scene_->activeCamera_]->resolution_;

	Ray ray;
	glm::vec3 color;
	glm::vec2 samplePosition = glm::vec2(0);
	int currentSample = 0;

	for (int y = 0; y < resolution.y; ++y) {
		for (int x = 0; x < resolution.x; ++x) {
			color = glm::vec3(0);
			for (currentSample = 0; currentSample < samplerPerPixel_; ++currentSample) {
				samplePosition = scene_->samplers_[scene_->activeSampler_]->getSamplePosition(x, y, *scene_->cameras_[scene_->activeCamera_], currentSample);
				ray = scene_->cameras_[scene_->activeCamera_]->generateRay(samplePosition);

				color = traceRay(ray, 0);
			}
			renderTexture.setPixelColor(x, y, ImVec4(color.r, color.g, color.b, 1.0f));
		}
	}
	renderTexture.updateTextureData();
}

glm::vec3 MonteCarloIntegrator::traceRay(Ray& r, int depth) {
	// Terminate recursion
	glm::vec3 hitColor = glm::vec3(0);
	glm::vec3 directColor = glm::vec3(0);
	glm::vec3 indirectColor = glm::vec3(0);
	glm::vec3 reflectedColor = glm::vec3(0);
	glm::vec3 refractedColor = glm::vec3(0);
	if (depth > maxRecursiveDepth_) {
		return glm::vec3(0);
	}

	// Get nearest object
	float distance = 0.0f;
	int objectIndex = getNearestIntersectionIndex(r, distance, 0);
	if (objectIndex != -1) {
		// Easy access to object
		Primitive* object = scene_->primitives_[objectIndex];
		Material* m = object->material_;
		r.material_ = m;
		// Get intersection point, normal and 
		glm::vec3 intersectionPoint = r.end_;
		glm::vec3 normal = object->shape_->getNormal(intersectionPoint);
		glm::vec3 offsetOrigin = intersectionPoint + normal * bias_;
		r.end_ = offsetOrigin;

		float reflectiveCoefficient = std::min(fresnel(r.direction_, normal, m->ior_) + m->reflectance_, 1.0f);
		float transmissiveCoefficient = 1.0f - reflectiveCoefficient;

		// Direct lighting
		if (m->transmittance_ > FLT_EPSILON) {
			directColor = (1.0f - (reflectiveCoefficient + transmissiveCoefficient)) * directLight(r);
		}
		else {
			directColor = directLight(r);
		}

		//if (m->reflectance_ > FLT_EPSILON) {
		//	// Calculate reflection direction
		//	glm::vec3 reflectionDirection = getReflectionDirection(r.direction_, normal);

		//	// Construct new ray and set pointers
		//	Ray refRay = Ray(offsetOrigin, reflectionDirection);
		//	r.reflected_ = std::make_shared<Ray>(refRay);
		//	refRay.parent_ = std::make_shared<Ray>(r);

		//	reflectedColor = m->reflectance_ * traceRay(refRay, ++depth) * std::max(glm::dot(reflectionDirection, normal), 0.0f);
		//}
		//else {
		//	r.reflected_ = nullptr;
		//}

		if (m->transmittance_ > FLT_EPSILON || m->reflectance_ > FLT_EPSILON) {


			bool outside = glm::dot(r.direction_, normal) < 0.0f;
			glm::vec3 bias = bias_ * normal;
			if (reflectiveCoefficient < 1.0f) {
				glm::vec3 refractionDirection = glm::normalize(getRefractionDirection(r.direction_, normal, m->ior_));
				glm::vec3 refractionOrigin = glm::vec3(0);
				if (outside) {
					refractionOrigin = r.end_ - bias;
				}
				else {
					refractionOrigin = r.end_ + bias;
				}
				Ray transmittedRay = Ray(refractionOrigin, refractionDirection);
				refractedColor = traceRay(transmittedRay, ++depth) * transmissiveCoefficient;
			}
			glm::vec3 reflectedDirection = getReflectionDirection(r.direction_, normal);
			glm::vec3 reflectedOrigin = glm::vec3(0);
			if (outside) {
				reflectedOrigin = r.end_ + bias;
			}
			else {
				reflectedOrigin = r.end_ - bias;
			}
			Ray reflectedRay = Ray(reflectedOrigin, reflectedDirection);
			reflectedColor = traceRay(reflectedRay, ++depth) * reflectiveCoefficient;
		}
		else {
			r.refracted_ = nullptr;
		}

		indirectColor = indirectLight(r, depth);
	}


	hitColor = ((directColor + reflectedColor + refractedColor) / 3.14f + 2.0f * indirectColor);
	return glm::clamp(hitColor, 0.0f, 1.0f);
}

int MonteCarloIntegrator::getNearestIntersectionIndex(Ray& r, float& depth, int type)
{
	float distance = std::numeric_limits<float>::max();
	float currentDistance;
	int index = -1;
	if (type == 0) { // Primitives
		for (int i = 0; i < scene_->primitives_.size(); ++i) {
			Ray ray = r;
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
			Ray ray = r;
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
	std::cout << "\n";
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

glm::vec3 MonteCarloIntegrator::indirectLight(const Ray& ray, int& depth)
{
	glm::vec3 color = glm::vec3(0);
	float pdf = 1.0f / (2.0f * 3.14f); // PDF = probability to sample specific direction in solid angle (current is hardcoded for full hemisphere)
	for (int n = 0; n < indirectLightSamplesOnHemisphere_; ++n) {
		float r1 = distribution(generator);
		float r2 = distribution(generator);

		glm::vec3 sampleDirection = uniformSampleHemisphere(ray.intersectionNormal_, r1, r2);
		Ray indirectRay = Ray(ray.end_, sampleDirection);
		// Trace indirect ray and adjust for incoming angle and propbability of specific ray
		color += traceRay(indirectRay, ++depth) * std::max(glm::dot(indirectRay.direction_, ray.intersectionNormal_), 0.0f) / pdf;
	}
	color /= static_cast<float>(indirectLightSamplesOnHemisphere_);
	return color;
}

glm::vec3 MonteCarloIntegrator::traverseTree(const Ray& root)
{
	int depth = 0;
	glm::vec3 color = glm::vec3(0);
	glm::vec3 directColor = glm::vec3(0);
	glm::vec3 indirectColor = glm::vec3(0);

	while (root.reflected_ != nullptr || root.refracted_ != nullptr) {
		directColor += (1.0f - root.material_->reflectance_) * directLight(root);
		indirectColor += indirectLight(root, ++depth);
	}
	color = directColor + indirectColor;
	return color;
}

void MonteCarloIntegrator::GUI() {
	ImGui::Text("Monte Carlo Integrator");
	ImGui::SliderInt("Max recursive depth", &maxRecursiveDepth_, 0, 10);
	ImGui::SliderInt("Indirect rays on hemisphere", &indirectLightSamplesOnHemisphere_, 0, 100);
	ImGui::SliderInt("Samples per light", &lightSamples_, 0, 100);
	Integrator::GUI();
}