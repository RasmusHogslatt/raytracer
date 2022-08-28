#pragma once

#include <Object.h>

Object::Object() : Actor(), material_{ Material() }, normal_{ glm::vec3(1,0,0) }, t{ std::numeric_limits<float>::max() } {
}

Object::Object(const Object& old)
{
	normal_ = old.normal_;
	material_ = old.material_;
}