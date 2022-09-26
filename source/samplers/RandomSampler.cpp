#pragma once

#include <samplers/RandomSampler.h>
#include <random>

std::default_random_engine gen;
std::uniform_real_distribution<float> distr(-1, 1);

RandomSampler::RandomSampler() : Sampler() {}

glm::vec2 RandomSampler::getSamplePosition(int x, int y, Camera& c, int currentSample)
{
	glm::vec2 center = getCenterSample(x, y, c);
	float random1 = distr(gen);
	float random2 = distr(gen);
	return glm::vec2(center.x + halfPixelSize_.x * random1, center.y + halfPixelSize_.y * random2);
}
