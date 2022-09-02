#pragma once

#include <core/Primitive.h>
#include <core/Ray.h>
#include <glm/glm.hpp>
#include <materials/Material.h>

class Sphere : public Primitive {
public:
	// Constructor
	Sphere(glm::vec3 position = glm::vec3(0), float radius = 1.0f);
	Sphere(const Sphere& old);

	// Virtual intersect
	bool intersect(Ray& r);
	glm::vec3 getPointOnHemisphere(float u, float v) const;
	void GUI();
public:
	glm::vec3 position_;
	float radius_;
};