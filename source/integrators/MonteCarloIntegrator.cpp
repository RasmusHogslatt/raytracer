#pragma once

#include <integrators/MonteCarloIntegrator.h>
#include <iostream>
#include <random>
#include <string>

std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(0, 1);

MonteCarloIntegrator::MonteCarloIntegrator(Scene* scene) : Integrator(scene), maxRecursiveDepth_{ 1 }, samplerPerPixel_{ 1 }, bias_{ 0.001f }, nHemisphereSamples_{ 5 } {}

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

				/*
				1. getNearestIntersectedObject()
				2. store intersection
				3. store normal
				4. castRay(position, normal, object*, depth)
				*/

				/*if (scene_->primitives_[0]->shape_->intersect(ray)) {
					color = scene_->primitives_[0]->material_->diffuse_;
					glm::vec3 intersectionPoint = ray.origin_;
					glm::vec3 normal = scene_->primitives_[0]->shape_->getNormal(intersectionPoint);
				}*/
				//trace

				// monte carlo
				/*
					1 - If hit object
						--- GATHERLIGHT START ---
						for(x amountOfRaySamples)
							randomDirection = getrandomdirection(normalvectorOfHitPointOnObject)
							Ray ray;
							ray.origin = intersectionPoint
							ray.direction = randomDirection
							indirectLight += traceRay(ray) // traceRay first calculates direct illumination, then use gather light after and returns sum of both
						end
						indirectLight /= amountOfRaySamples
						--- GATHERLIGHT END ---
				*/
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

		// Get intersection point, normal and 
		glm::vec3 intersectionPoint = r.origin_ + distance * r.direction_;
		glm::vec3 normal = object->shape_->getNormal(intersectionPoint);
		glm::vec3 offsetOrigin = intersectionPoint + normal * bias_;
		//r.origin_ = offsetOrigin;

		// Direct lighting
		for (int i = 0; i < scene_->lights_.size(); ++i) {
			Light* light = scene_->lights_[i];
			for (int lightSamples = 0; lightSamples < 10; ++lightSamples) {

				float r1 = distribution(generator);
				float r2 = distribution(generator);
				glm::vec3 pointOnLightHemisphere = uniformSampleHemisphere(glm::normalize(intersectionPoint - light->shape_->getPosition()), r1, r2);
				glm::vec3 lightDirection = glm::normalize(intersectionPoint - pointOnLightHemisphere);
				Ray lightRay = Ray(offsetOrigin, lightDirection);
				float x;
				if (object->shape_->intersect(lightRay, x)) {
					directColor += m->color_ * (1.0f - m->reflectance_) * std::max(glm::dot(-lightDirection, normal), 0.0f) / 5.0f;
				}
			}
			if (m->reflectance_ > FLT_EPSILON) {
				glm::vec3 reflectionDirection = getReflectionDirection(r.direction_, normal);
				Ray refRay = Ray(offsetOrigin, reflectionDirection);
				directColor += m->reflectance_ * traceRay(refRay, ++depth) * std::max(glm::dot(reflectionDirection, normal), 0.0f);

			}
		}

		float pdf = 1.0f / (2.0f * 3.14f); // PDF = probability to sample specific direction in solid angle (current is hardcoded for full hemisphere)
		for (int n = 0; n < nHemisphereSamples_; ++n) {
			float r1 = distribution(generator);
			float r2 = distribution(generator);

			glm::vec3 sampleDirection = uniformSampleHemisphere(normal, r1, r2);
			Ray indirectRay = Ray(offsetOrigin, sampleDirection);
			// Trace indirect ray and adjust for incoming angle and propbability of specific ray
			indirectColor += traceRay(indirectRay, ++depth) * std::max(glm::dot(indirectRay.direction_, normal), 0.0f) / pdf;
		}
		indirectColor /= static_cast<float>(nHemisphereSamples_);
	}




	// DEBUG
	hitColor = (directColor / 3.14f + 2.0f * indirectColor);
	//return glm::clamp(glm::vec3(distance/20.0f), 0.0f, 1.0f);// Sphere intersection is picking furthest point
	return glm::clamp(hitColor, 0.0f, 1.0f);

	/*
	vec3f castRay(Vec2f P, Vec2f N, uin32_t depth) {
	if (depth > scene->options.maxDepth) return 0;
	Vec2f N = ..., P = ...;  //normal and position of the shaded point
	Vec3f directLightContrib = 0, indirectLightContrib = 0;

	// compute direct illumination
	for (uint32_t i = 0; i < scene->nlights; ++i) {
		Vec2f L = scene->lights[i].getLightDirection(P);
		Vec3f L_i = scene->lights[i]->intensity * scene->lights[i]->color;
		// we assume the surface at P is diffuse
		directLightContrib += shadowRay(P, -L) * std::max(0.f, N.dotProduct(L)) * L_i;
	}

	// compute indirect illumination
	float rotMat[2][2] = {{N.y, -N.x}, {N.x, N.y}};  //compute a rotation matrix
	uint32_t N = 16;
	for (uint32_t n = 0; n < N; ++n) {
		// step 1: draw a random sample in the half-disk
		float theta = drand48() * M_PI;
		float cosTheta = cos(theta);
		float sinTheta = sin(theta);
		// step 2: rotate the sample direction
		float sx = cosTheta  * rotMat[0][0] + sinTheta  * rotMat[0][1];
		float sy = cosTheta  * rotMat[1][0] + sinTheta  * rotMat[1][1];
		// step 3: cast the ray into the scene
		Vec3f sampleColor = castRay(P, Vec2f(sx, sy), depth + 1);  //trace a ray from P in random direction
		// step 4 and 5: treat the return color as if it was a light (we assume our shaded surface is diffuse)
		IndirectLightContrib += sampleColor * cosTheta;  //diffuse shading = L_i * cos(N.L)
	}
	// step 6: divide the result of indirectLightContrib by the number of samples N (Monte Carlo integration)
	indirectLightContrib /= N;

	// final result is diffuse from direct and indirect lighting multiplied by the object color at P
	return (indirectLightContrib + directLightContrib) * objectAlbedo / M_PI;
}
	*/
	return glm::vec3(0);
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

void MonteCarloIntegrator::GUI() {
	ImGui::Text("Monte Carlo Integrator");
	ImGui::SliderInt("Max recursive depth", &maxRecursiveDepth_, 0, 10);
	ImGui::SliderInt("Samples on hemisphere", &nHemisphereSamples_, 0, 100);
}