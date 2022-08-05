#pragma once

#include <glm/glm.hpp>

class Actor {
public:
	Actor(const glm::vec3& pos = glm::vec3(0, 0, 0));

	virtual bool intersect() = 0;
protected:
	glm::vec3 position_;
};