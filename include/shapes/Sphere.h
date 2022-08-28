#pragma once

#include <Object.h>

class Sphere : public Object {
public:
	Sphere(float radius = 1.0);
	Sphere(const Sphere& old);
	bool intersect(const Ray& ray, float& distance);
	void GUISettings();
public:
	float radius_;
};