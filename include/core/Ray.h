#pragma once

#include <glm/glm.hpp>

class Ray {
public:
	Ray(const glm::vec3& o = glm::vec3(0), const glm::vec3& d = glm::vec3(0));

public:
	glm::vec3 origin_;
	glm::vec3 end_;
	glm::vec3 direction_;

	// RGB vector, importance starts at 1, and drops of relating with reflectance of intersected objects
	glm::vec3 importance_;
};