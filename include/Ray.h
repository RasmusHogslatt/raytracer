#pragma once

#include <glm/glm.hpp>

class Ray {
public:
	Ray(const glm::vec3& o = glm::vec3(0), const glm::vec3& d = glm::vec3(0, 0, 1));
	glm::vec3 getPositionAlongRay(float t);
public:
	glm::vec3 origin;
	glm::vec3 direction;
};