#pragma once

#include <Object.h>

class Sphere : public Object {
public:
	Sphere(float radius = 1.0);

	bool intersect();
private:
	float radius_;
};