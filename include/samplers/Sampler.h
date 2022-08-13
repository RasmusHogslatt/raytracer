#pragma once

#include <glm/glm.hpp>

class Sampler {
public:
	virtual ~Sampler();
	Sampler(size_t samplesPerPixel);
	virtual void startPixel(const glm::vec2& pixel);
	virtual float get1D() = 0;
	virtual glm::vec2 get2D() = 0;
	CameraSample getCameraSample()
};