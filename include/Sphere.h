#pragma once

#include <Object.h>

class Sphere : public Object {
public:
	Sphere(float radius = 1.0);

	bool intersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection);
private:
	float radius_;
};