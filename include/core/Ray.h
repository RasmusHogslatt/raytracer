#pragma once

#include <glm/glm.hpp>
#include <materials/Material.h>

class IntersectionData {
public:
	IntersectionData();
	~IntersectionData();
public:
	Material* material_ = nullptr;
	glm::vec3 normal_ = glm::vec3(0);
	glm::vec3 position_ = glm::vec3(0);
};

class Ray {
public:
	Ray(const glm::vec3& o = glm::vec3(0), const glm::vec3& d = glm::vec3(0));
	~Ray();
public:
	glm::vec3 origin_;
	glm::vec3 direction_;
	// RGB vector, importance starts at 1, and drops of relating with reflectance of intersected objects
	glm::vec3 importance_;
};