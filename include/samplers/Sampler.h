#pragma once

#include <Parameters.h>
#include <glm/glm.hpp>
#include <MyCamera.h>

class Sampler {
public:
	virtual glm::vec2 getSamplePosition(int x, int y, MyCamera& c, int currentSample) = 0;
};