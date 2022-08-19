#pragma once

#include <glm/glm.hpp>
#include <Ray.h>

class Actor {
public:
	Actor(const glm::vec3& pos = glm::vec3(0, 0, 0));
	Actor(const Actor& old);

	virtual bool intersect(const Ray& ray) = 0;
	virtual void GUISettings() = 0;
public:
	glm::vec3 position_;
};