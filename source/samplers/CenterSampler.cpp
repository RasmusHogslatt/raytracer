#pragma once

#include <samplers/CenterSampler.h>

CenterSampler::CenterSampler() : Sampler() {}

glm::vec2 CenterSampler::getSamplePosition(int x, int y, Camera& c, int currentSample) {
	return getCenterSample(x, y, c);
}