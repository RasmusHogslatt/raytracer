#pragma once

#include "DebugRenderer.h"

DebugRenderer::DebugRenderer(Parameters& params) : p{ params } {}

void DebugRenderer::Render() {
	int width = p.renderTexture_.getWidth();
	int height = p.renderTexture_.getHeight();
	glm::vec3 color = glm::vec3(0);
	glm::vec2 xyPos = glm::vec2(0);
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			for (int currentSample = 0; currentSample < p.samples_; ++currentSample) {
				xyPos = getSamplePosition(x, y, currentSample);
				color = p.camera.generateRay(xyPos.x, xyPos.y);
			}

			p.renderTexture_.setPixelColor(x, y, ImVec4(color.r, color.g, color.b, 1));
		}
	}

	p.renderTexture_.updateTextureData();
}

void DebugRenderer::RenderPixel() {
	p.renderTexture_.setPixelColor(p.currentx, p.currenty, ImVec4(1, 1, 1, 1));

	p.renderTexture_.updateTextureData();
}

void DebugRenderer::setBackgroundColor(ImVec4 color) {
	p.renderTexture_.clearTextureColor(color);
}

glm::vec2 DebugRenderer::getSamplePosition(int x, int y, int currentSample)
{
	// Check correct calculations based aspectratio, fov etc. Is a pixel truly 1:1 in size? or w/h and h/w respectively
	glm::vec2 pos = glm::vec2(0);
	float width = p.camera.resolution.x;
	float height = p.camera.resolution.y;
	float aspectRatio = width / height;

	// Center of pixel
	pos.x = (static_cast<float>(x) / width) - 0.5;
	pos.y = (static_cast<float>(y) / height) - 0.5;

	// Single sample in center of pixel
	if (p.sampleMode == 0) {
		return pos;
	}
	// Random position within pixel
	else if (p.sampleMode == 1) {
		float r = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
		pos.x += r/width;
		pos.y += r/height;
	}
	return pos;
}
