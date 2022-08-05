#pragma once

#include <Actor.h>
#include <Material.h>

class Object : public Actor {
public:
	Object();

	virtual bool intersect() = 0;
protected:
	glm::vec3 normal_;
	Material material_;
};