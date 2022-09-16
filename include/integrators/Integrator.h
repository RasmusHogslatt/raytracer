#pragma once

#include <core/Scene.h>
#include <core/Texture.h>

class Integrator {
public:
	Integrator(Scene* scene);

	virtual void Integrate(Texture& renderTexture) = 0;
	virtual void GUI();
	glm::vec3 getReflectionDirection(const glm::vec3& incident, const glm::vec3& normal) const;
	glm::vec3 getRefractionDirection(const glm::vec3& incident, const glm::vec3& normal, float ior) const;

	// Returns amount of reflection Kr. Transmittance Kt is given by Kt = 1 - Kr
	float fresnel(const glm::vec3& incident, const glm::vec3& normal, float ior);

public:
	Scene* scene_;
	float globalIOR_;
};