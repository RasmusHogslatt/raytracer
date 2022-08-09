#pragma once

#include <Actor.h>
#include <Material.h>

class Object : public Actor {
public:
	Object();

	virtual bool intersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection) = 0;
protected:
	glm::vec3 normal_;
	Material material_;
};