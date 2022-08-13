#pragma once

#include <Object.h>

class MySphere : public Object {
public:
	MySphere(float radius = 1.0);

	bool intersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection);
private:
	float radius_;
};