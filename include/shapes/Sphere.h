#pragma once

#include <shapes/Shape.h>

class Sphere : public Shape {
public:
	Sphere(float radius);
	bool intersect(Ray& ray, float* tHit) const;
private:
	float radius;

};