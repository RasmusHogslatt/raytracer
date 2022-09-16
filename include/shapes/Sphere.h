#pragma once

#include <shapes/Shape.h>
#include <core/Ray.h>
#include <glm/glm.hpp>
#include <materials/Material.h>

class Sphere : public Shape {
public:
	// Constructor
	Sphere(glm::vec3 position = glm::vec3(0), float radius = 1.0f);
	Sphere(const Sphere& old);
	Sphere* clone() const;
	~Sphere();

	// Checks for intersection. If found, sets ray.origin to intersection point
	bool intersect(Ray& ray, float& t);
	glm::vec3 getPosition();
	float getArea();
	glm::vec3 getNormal(const glm::vec3& intersectionPoint);
	glm::vec3 getPointOnSurface(const glm::vec3& normal, float u, float v);
	glm::vec3 getPointOnHemisphere(float u, float v) const;
	void GUI();
public:
	glm::vec3 position_;
	float radius_;
	float area_;
};