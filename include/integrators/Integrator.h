#pragma once

#include <core/Scene.h>
#include <core/Texture.h>
#include <random>

class Integrator {
public:
	Integrator(Scene* scene);

	virtual void Integrate(Texture& renderTexture) = 0;
	virtual void GUI();
	glm::vec3 getReflectionDirection(const glm::vec3& incident, const glm::vec3& normal) const;
	glm::vec3 getRefractionDirection(const glm::vec3& incident, const glm::vec3& normal, float n1, float n2, bool& TIR) const;
	
	// Returns amount of reflection Kr. Transmittance Kt is given by Kt = 1 - Kr. Takes material IOR and checks inside outside itself
	float schlickApproximation(const glm::vec3& incident, const glm::vec3& normal, float n1, float n2);
	float getIORRatio(const Ray& ray);
public:
	Scene* scene_;
	float globalIOR_;
	int samplerPerPixel_;
};