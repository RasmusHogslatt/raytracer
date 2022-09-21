#pragma once

#include <integrators/Integrator.h>
#include <imgui.h>

Integrator::Integrator(Scene* scene) : scene_{ scene }, globalIOR_{ 1.0f } {}

void Integrator::GUI()
{
	ImGui::SliderFloat("Global IOR", &globalIOR_, 0.1f, 4.3f);
}

glm::vec3 Integrator::getReflectionDirection(const glm::vec3& incident, const glm::vec3& normal) const
{
	return glm::normalize(incident - 2.0f * glm::dot(incident, normal) * normal);
}

glm::vec3 Integrator::getRefractionDirection(const glm::vec3& incident, const glm::vec3& normal, float n1, float n2, bool& TIR) const
{
	TIR = false;
	const float n = n1 / n2;
	const float cosI = glm::dot(normal, incident);
	const float sinT2 = n * n * (1.0f - cosI * cosI);
	if (sinT2 > 1.0f) {
		TIR = true;
		return glm::vec3(0); // TIR
	}
	const float cosT = sqrt(1.0f - sinT2);
	return n * incident + (n * cosI - cosT) * normal;
}

float Integrator::fresnel(const glm::vec3& incident, const glm::vec3& normal, float n1, float n2)
{
	// Schlick aproximation
	float r0 = (n1 - n2) / (n1 + n2);
	r0 *= r0;
	float cosX = -glm::dot(normal, incident);
	if (n1 > n2)
	{
		float n = n1 / n2;
		float sinT2 = n * n * (1.0 - cosX * cosX);
		// Total internal reflection
		if (sinT2 > 1.0)
			return 1.0f;
		cosX = sqrt(1.0 - sinT2);
	}
	float x = 1.0 - cosX;
	float ret = r0 + (1.0 - r0) * x * x * x * x * x;
	return ret;
}

float Integrator::schlick(const glm::vec3& incident, const glm::vec3& normal, float etai, float etat)
{
	float cosTheta = glm::dot(incident, normal);
	float r0 = std::pow(((etat - etai) / (etat + etai)), 2);
	return r0 + (1.0f - r0) * std::pow((1.0f - cosTheta), 5);
}

float Integrator::getIORRatio(const Ray& ray)
{
	if (ray.inside_) {
		return ray.material_->ior_ / globalIOR_;
	}
	else {
		return globalIOR_ / ray.material_->ior_;
	}
}
