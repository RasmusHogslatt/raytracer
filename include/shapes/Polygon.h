#pragma once

#include <shapes/Shape.h>
#include <vector>

class Polygon : public Shape {
public:
	Polygon();
	~Polygon();
	Polygon* clone() const = 0;

	virtual bool intersect(Ray& ray, float& t) = 0;
	virtual glm::vec3 getPosition() = 0;
	virtual glm::vec3 getNormal(const glm::vec3& intersectionPoint) = 0;
	virtual float getArea() = 0;
	virtual void GUI() = 0;
	glm::vec3 getPointOnSurface(const glm::vec3& normal, float u, float v) = 0;
public:
	std::vector<glm::vec3> vertices_;
	glm::vec3 normal_;
};