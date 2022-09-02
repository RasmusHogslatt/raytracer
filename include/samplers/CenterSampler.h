#pragma once

#include <samplers/Sampler.h>

class CenterSampler : public Sampler {
public:
	CenterSampler();
	glm::vec2 getSamplePosition(int x, int y, Camera& c, int currentSample);
};