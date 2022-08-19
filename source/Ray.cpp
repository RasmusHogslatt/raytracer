#pragma once

#include <Ray.h>

Ray::Ray(const glm::vec3& o, const glm::vec3& d) : origin{ o }, direction{ glm::normalize(d) }
{
}

glm::vec3 Ray::getPositionAlongRay(float t)
{
	return origin + t * direction;
}
