#pragma once

#include <samplers/CenterSampler.h>

CenterSampler::CenterSampler()
{
}

glm::vec2 CenterSampler::getSamplePosition(int x, int y, MyCamera& c, int currentSample)
{
	glm::vec2 pos = glm::vec2(0);
	float width = c.resolution.x;
	float height = c.resolution.y;
	float aspectRatio = width / height;

	pos.x = (static_cast<float>(x) / width) - 0.5;
	pos.y = (static_cast<float>(y) / height) - 0.5;
	return pos;
}
