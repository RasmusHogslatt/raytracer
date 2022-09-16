#pragma once

#include <core/Ray.h>

Ray::Ray(const glm::vec3& o, const glm::vec3& d, std::shared_ptr<Ray> parent) : origin_{ o }, direction_{ glm::normalize(d) }, importance_{ glm::vec3(1.0f) }, parent_{ parent }, end_{ glm::vec3(0.0f) }, material_{ nullptr }, reflected_{ nullptr }, refracted_{ nullptr }, intersectionNormal_{ glm::vec3(0) }
{
}

Ray::~Ray()
{
}

void Ray::setEnd(float t)
{
	end_ = origin_ + direction_ * t;
}
