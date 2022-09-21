#pragma once

#include <shapes/Polygon.h>

class Rectangle : public Polygon {
public:
	Rectangle(const glm::vec3 bl = glm::vec3(-1, -1, 0),
		const glm::vec3 tl = glm::vec3(-1, 1, 0),
		const glm::vec3 tr = glm::vec3(1, 1, 0));
	Rectangle(const Rectangle& old);
	~Rectangle();
	Rectangle* clone() const;

	bool intersect(Ray& ray, float& t);

	glm::vec3 getPosition();
	glm::vec3 getNormal(const glm::vec3& intersectionPoint);
	float getArea();
	glm::vec3 getPointOnSurface(const glm::vec3& normal, float u, float v);
	void GUI();
public:
	float area_;
};