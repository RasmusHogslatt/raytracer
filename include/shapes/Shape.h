#pragma once

#include <glm/glm.hpp>
#include <core/Ray.h>

class Shape {
public:
	Shape();
	
	virtual Shape* clone() const = 0;
	virtual ~Shape();
	// True if intersected, sets ray.end = intersection point

	// Checks for intersection. If found, sets ray.origin to intersection point
	virtual bool intersect(Ray& ray, float& t) = 0;
	virtual glm::vec3 getPosition() = 0;
	virtual glm::vec3 getNormal(const glm::vec3& intersectionPoint) = 0;
	virtual float getArea() = 0;
	virtual void GUI();
	virtual glm::vec3 getPointOnSurface(const glm::vec3& normal, float u, float v) = 0;
};
