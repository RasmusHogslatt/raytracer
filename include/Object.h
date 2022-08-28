#pragma once

#include <Actor.h>
#include <materials/Material.h>

class Object : public Actor {
public:
	Object();
	Object(const Object& old);

	virtual bool intersect(const Ray& ray, float& distance) = 0;
	virtual void GUISettings() = 0;
public:
	float t;

	glm::vec3 normal_;
	Material material_;
};