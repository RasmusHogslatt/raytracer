#pragma once

#include <samplers/RandomSampler.h>
#include <random>

RandomSampler::RandomSampler()
{
}

glm::vec2 RandomSampler::getSamplePosition(int x, int y, Camera& c, int currentSample)
{
	glm::vec2 center = getCenterSample(x, y, c);
	return center;
}
