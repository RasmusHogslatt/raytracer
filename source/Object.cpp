#pragma once

#include <Object.h>

Object::Object() : Actor(), normal_{glm::vec3(1,0,0)}, material_{Material()} {
}

Object::Object(const Object& old)
{
	normal_ = old.normal_;
	material_ = old.material_;
}
