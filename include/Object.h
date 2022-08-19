#pragma once

#include <Actor.h>
#include <materials/Material.h>

class Object : public Actor {
public:
	Object();
	Object(const Object& old);

	virtual bool intersect(const Ray& ray) = 0;
	virtual void GUISettings() = 0;
public:
	glm::vec3 normal_;
	Material material_;
};