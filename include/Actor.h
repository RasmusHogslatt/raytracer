#pragma once

#include <glm/glm.hpp>

class Actor {
public:
	Actor(const glm::vec3& pos = glm::vec3(0, 0, 0));

	virtual bool intersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) = 0;
protected:
	glm::vec3 position_;
};