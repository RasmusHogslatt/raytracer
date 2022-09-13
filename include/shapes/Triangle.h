#pragma once

#include <shapes/Polygon.h>

class Triangle : public Polygon {
public:
	Triangle(const glm::vec3& v0 = glm::vec3(-1, 0, 0), const glm::vec3& v1 = glm::vec3(0, 1, 0), const glm::vec3& v2 = glm::vec3(1, 0, 0));
	Triangle(const Triangle& old);
	~Triangle();

	Triangle* clone() const;

	bool intersect(Ray& ray, float& t);

	// Mean position of vertices
	glm::vec3 getPosition();
	glm::vec3 getNormal(const glm::vec3& intersectionPoint);
	float getArea();
	void GUI();
public:
	float area_;
};