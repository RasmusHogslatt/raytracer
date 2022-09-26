#pragma once

#include <shapes/Shape.h>
#include <shapes/Triangle.h>
#include <vector>

class Tetrahedron : public Shape {
public:
	Tetrahedron(const glm::vec3& v1 = glm::vec3(sqrt(8.0f/9.0f), -1.0f/3.0f, 0.0f), const glm::vec3& v2 = glm::vec3(-sqrt(2.0f/9.0f), -1.f/3.f, sqrt(2.f/3.f)), const glm::vec3& v3 = glm::vec3(-sqrt(2.f/9.f), -1.f/3.f, -sqrt(2.f/3.f)), const glm::vec3& v4 = glm::vec3(0.0f, 1.0f, 0.0f));

	Tetrahedron* clone() const;

	bool intersect(Ray& ray, float& t);
	glm::vec3 getPosition();
	glm::vec3 getNormal(const glm::vec3& intersectionPoint);
	float getArea();
	void GUI();
	glm::vec3 getPointOnSurface(const glm::vec3& normal, float u, float v);
public:
	float area_;
	int intersectedIndex_;
	std::vector<Triangle*> triangles_;
};