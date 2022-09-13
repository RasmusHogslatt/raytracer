#pragma once

#include <core/Ray.h>

Ray::Ray(const glm::vec3& o, const glm::vec3& d) : origin_{ o }, direction_{ glm::normalize(d) }, importance_{ glm::vec3(1.0f) }
{
}

Ray::~Ray()
{
}

IntersectionData::IntersectionData()
{
}

IntersectionData::~IntersectionData()
{
	material_ = nullptr;
}
