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

glm::vec3 Integrator::getRefractionDirection(const glm::vec3& incident, const glm::vec3& normal, float ior) const
{
	glm::vec3 n = normal;
	float innerIOR = ior;
	float globalIOR = globalIOR_;

	float cosi = glm::clamp(glm::dot(incident, normal), -1.0f, 1.0f);

	if (cosi < 0.0f) {
		cosi = -cosi;
	}
	else {
		std::swap(innerIOR, globalIOR);
		n = -normal;
	}
	float resultingIOR = innerIOR / globalIOR;
	float k = (1.0f - resultingIOR * resultingIOR) * (1.0f - cosi * cosi);

	if (k < 0.0f) {
		return glm::vec3(0);
	}
	else {
		return (resultingIOR * incident + (resultingIOR * cosi - std::sqrt(k)) * n);
	}
}

float Integrator::fresnel(const glm::vec3& incident, const glm::vec3& normal, float ior)
{
	float kr = 0.0f;
		float cosi = glm::clamp(glm::dot(incident, normal), -1.0f, 1.0f);
	float innerIOR = ior;
	float globalIOR = globalIOR_;

	if (cosi > 0.0f) {
		std::swap(innerIOR, globalIOR);
	}

	float sint = innerIOR / globalIOR * std::sqrt(std::max(0.0f, 1.0f - cosi * cosi));

	if (sint >= 1.0f) {
		kr = 1.0f;
		return kr;
	}
	else {
		float cost = std::sqrt(std::max(0.0f, 1.0f - sint * sint));
		cosi = std::fabs(cosi);
		float Rs = ((globalIOR * cosi) - (innerIOR * cost)) / ((globalIOR * cosi) + (innerIOR * cost));
		float Rp = ((innerIOR * cosi) - (globalIOR * cost)) / ((innerIOR * cosi) + (globalIOR * cost));
		kr = (Rs * Rs + Rp * Rp) / 2.0f;
		return kr;
	}
}
