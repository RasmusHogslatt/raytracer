#pragma once

#include <integrators/Integrator.h>
#include <imgui.h>

Integrator::Integrator(Scene* scene) : bias{ 0.0001f }, maxRecursiveDepth{ 1 }, scene{ scene }, globalIOR{ 1.0f }, currentRecurseDepth{ maxRecursiveDepth }
{
}

void Integrator::GUISettings()
{
	ImGui::SliderFloat("Offset Bias [10^-4]", &bias, 1, 10, " % .3f", -4.0f);
	ImGui::SliderFloat("Global IOR", &globalIOR, 0.001f, 4.0f);
	ImGui::SliderInt("Recursive depth", &maxRecursiveDepth, 0, 20);
}

void Integrator::offsetIntersectionPoint(Ray& offsetRay, const glm::vec3& normal, float bias)
{
	offsetRay.origin += normal * bias;
}


Ray& Integrator::getReflectionRay(const Ray& ray, const glm::vec3& normal)
{
	// Create new ray
	Ray reflectedRay = ray; //origin must be intersection point
	// Offset its origin with bias
	offsetIntersectionPoint(reflectedRay, normal, bias);
	// Calculate it's reflection direction
	reflectedRay.direction = ray.direction - 2.0f * normal * glm::dot(ray.direction, normal);
	reflectedRay.direction = glm::normalize(reflectedRay.direction);
	return reflectedRay;
}

Ray& Integrator::getRefractionRay(const Ray& ray, const glm::vec3& normal, float ior) // Check if direction and normal to check if entering or leaving
{
	// Create new ray
	Ray refractedRay = ray;
	// Offset its origin with bias
	offsetIntersectionPoint(refractedRay, -normal, bias);
	float cosi = glm::clamp(-1.0f, 1.0f, glm::dot(ray.direction, normal));
	float prevIOR = globalIOR;
	float nextIOR = ior;
	if (cosi < 0) { // Entering 
		cosi = -cosi;
	}
	else { // Exiting
		std::swap(prevIOR, nextIOR);
	}
	Ray r = Ray();
	return r;
}

glm::vec3 Integrator::phongShading(Object* object, const glm::vec3& normal)
{
	glm::vec3 diffuse = glm::vec3(0);
	glm::vec3 specular = glm::vec3(0);
	glm::vec3 lightDirection = glm::vec3(0);
	for (int i = 0; i < scene->lights.size(); ++i) {
		lightDirection = glm::normalize(scene->lights[i]->position_ - object->position_);

		diffuse += object->material_.getDiffuseRGB() * object->material_.getDiffuseStrength() * glm::max(glm::dot(lightDirection, normal), 0.0f) * scene->lights[i]->lightProperties_.color * scene->lights[i]->lightProperties_.intensity;

		glm::vec3 viewDirection = glm::normalize(scene->cameras_[scene->activeCamera]->position - object->position_);

		glm::vec3 reflectionDirection = glm::normalize(2.0f * (glm::dot(normal, lightDirection)) * normal - lightDirection);

		specular = object->material_.getSpecularRGB() * object->material_.getSpecularStrength() * std::pow(glm::dot(viewDirection, reflectionDirection), object->material_.shininess) * scene->lights[i]->lightProperties_.color * scene->lights[i]->lightProperties_.intensity;
	}
	return diffuse + specular;
}

Material& Integrator::getMaterial(Object* object)
{
	return object->material_;
}

int Integrator::getClosestObjectIndex(const Ray& ray)
{
	float distance = std::numeric_limits<float>::max();
	float currentDistance;
	int closestObject = -1;

	// Find next closest object from ray origin
	for (int i = 0; i < scene->primitives.size(); ++i) {
		if (scene->primitives[i]->intersect(ray, currentDistance)) {
			// Update current closest intersection
			if (currentDistance < distance) {
				closestObject = i;
				distance = currentDistance;
			}
		}
	}
	return closestObject;
}
