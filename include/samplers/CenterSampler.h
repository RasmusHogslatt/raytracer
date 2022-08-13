#pragma once

#include <Parameters.h>
#include <samplers/Sampler.h>
class CenterSampler : public Sampler {
public:
	CenterSampler();
	glm::vec2 getSamplePosition(int x, int y, MyCamera& c, int currentSample);
};