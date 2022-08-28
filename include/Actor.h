#pragma once

#include <glm/glm.hpp>
#include <Ray.h>
#include <materials/Material.h>

class Actor {
public:
	Actor(const glm::vec3& pos = glm::vec3(0, 0, 0));
	Actor(const Actor& old);

	virtual void GUISettings() = 0;
public:
	glm::vec3 position_;
};