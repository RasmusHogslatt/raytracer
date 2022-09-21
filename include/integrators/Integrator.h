#pragma once

#include <core/Scene.h>
#include <core/Texture.h>

class Integrator {
public:
	Integrator(Scene* scene);

	virtual void Integrate(Texture& renderTexture) = 0;
	virtual void GUI();
	glm::vec3 getReflectionDirection(const glm::vec3& incident, const glm::vec3& normal) const;
	glm::vec3 getRefractionDirection(const glm::vec3& incident, const glm::vec3& normal, float n1, float n2, bool& TIR) const;
	
	// Returns amount of reflection Kr. Transmittance Kt is given by Kt = 1 - Kr. Takes material IOR and checks inside outside itself
	float fresnel(const glm::vec3& incident, const glm::vec3& normal, float n1, float n2);
	float schlick(const glm::vec3& incident, const glm::vec3& normal, float etai, float etat);
	float getIORRatio(const Ray& ray);
public:
	Scene* scene_;
	float globalIOR_;
};