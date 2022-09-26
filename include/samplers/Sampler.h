#pragma once

#include <glm/glm.hpp>
#include <cameras/Camera.h>

class Sampler {
public:
	Sampler();
	virtual glm::vec2 getSamplePosition(int x, int y, Camera& c, int currentSample) = 0;
	glm::vec2 getCenterSample(int x, int y, Camera& c);

	virtual void GUI();
public:
	glm::vec2 halfPixelSize_;
};