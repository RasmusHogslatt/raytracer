#pragma once

#include <core/Ray.h>

Ray::Ray(const glm::vec3& o, const glm::vec3& d) : origin_{ o }, direction_{ glm::normalize(d) }, end_{ glm::vec3(0) }, importance_{ glm::vec3(1) }
{
}
