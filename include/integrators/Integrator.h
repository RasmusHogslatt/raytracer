#pragma once

#include <core/Scene.h>
#include <core/Texture.h>

class Integrator {
public:
	Integrator(Scene* scene);

	virtual void Integrate(Texture& renderTexture) = 0;
	virtual void GUI() = 0;
	glm::vec3 getReflectionDirection(const glm::vec3& incident, const glm::vec3& normal) const;

public:
	Scene* scene_;
};