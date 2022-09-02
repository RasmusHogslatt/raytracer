#pragma once

#include <glm/glm.hpp>
#include <core/Ray.h>

class Shape {
public:
	Shape();
	Shape(const Shape& old);
	// True if intersected, sets ray.end = intersection point
	virtual bool intersect(Ray& r) = 0;
	virtual void GUI();
public:
	glm::vec3 position_;
};
