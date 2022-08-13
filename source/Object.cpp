#pragma once

#include "Object.h"

Object::Object() : Actor(), normal_{glm::vec3(1,0,0)}, material_{MyMaterial()} {
}
