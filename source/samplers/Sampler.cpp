#pragma once

#include <samplers/Sampler.h>

glm::vec2 Sampler::getSamplePosition(int x, int y, Camera& c, int currentSample)
{
	// Prerequisites
	glm::vec2 pos = glm::vec2(0);
	float width = static_cast<float>(c.resolution.x);
	float height = static_cast<float>(c.resolution.y);
	float aspectRatio = width / height;

	// Pixel space x: [0, width] y: [0, height]
	float pixelx = static_cast<float>(x);
	float pixely = static_cast<float>(y);

	// Normalized device coordinates x: [0, 1] y: [0, 1]
	float NDCx = (pixelx + 0.5f) / (width);
	float NDCy = (pixely + 0.5f) / height;

	// Screen space x: [-1, 1] y: [-1, 1]
	float screenx = 2.0f * NDCx - 1.0f;
	float screeny = 1.0f - 2.0f * NDCy;

	// Camera space x: [-aspectRatio, aspectRatio] y: [-1, 1]
	float camerax = screenx * aspectRatio;
	float cameray = screeny;

	// Set sample position
	pos.x = camerax;
	pos.y = cameray;

	return pos;
}

glm::vec2 Sampler::getCenterSample(int x, int y, Camera& c)
{
	// Prerequisites
	glm::vec2 pos = glm::vec2(0);
	float width = static_cast<float>(c.resolution.x);
	float height = static_cast<float>(c.resolution.y);
	

	// Pixel space x: [0, width] y: [0, height]
	float pixelx = static_cast<float>(x);
	float pixely = static_cast<float>(y);

	// Normalized device coordinates x: [0, 1] y: [0, 1]
	float NDCx = (pixelx + 0.5f) / (width);
	float NDCy = (pixely + 0.5f) / height;

	// Screen space x: [-1, 1] y: [-1, 1]
	float screenx = 2.0f * NDCx - 1.0f;
	float screeny = 1.0f - 2.0f * NDCy;

	// Camera space x: [-aspectRatio, aspectRatio] y: [-1, 1]
	float camerax = screenx * c.aspectRatio;
	float cameray = screeny;

	// Set sample position
	pos.x = camerax;
	pos.y = cameray;

	return pos;
}
