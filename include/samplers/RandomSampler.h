#pragma once

#include <samplers/Sampler.h>

class RandomSampler : public Sampler {
public:
	RandomSampler();
	glm::vec2 getSamplePosition(int x, int y, Camera& c, int currentSample);
};