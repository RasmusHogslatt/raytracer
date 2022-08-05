#pragma once

#include <glm/glm.hpp>

struct Material {
	glm::vec4 color_ = glm::vec4(1,0,0,1);
	float ior_ = 1.0;
};