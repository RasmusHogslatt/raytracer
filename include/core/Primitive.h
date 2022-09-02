#pragma once

#include <materials/Material.h>
#include <core/Ray.h>

class Primitive {
public:
	Primitive(Material* material = new Material());
	Primitive(const Primitive& old);
	virtual ~Primitive();
	// Checks for intersection. If found, sets ray.origin to intersection point
	virtual bool intersect(Ray& r) = 0;
	virtual void GUI() = 0;
public:
	Material* material_;
};