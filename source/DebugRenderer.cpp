#pragma once

#include "DebugRenderer.h"

DebugRenderer::DebugRenderer(Parameters& params) : p{ params } {}

void DebugRenderer::Render() {
	int width = p.renderTexture_.getWidth();
	int height = p.renderTexture_.getHeight();
	glm::vec4 color = glm::vec4(0);
	glm::vec2 xyPos = glm::vec2(0);
	glm::vec3 ray;
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			for (int currentSample = 0; currentSample < p.samples_; ++currentSample) {
				xyPos = getSamplePosition(x, y, currentSample);
				ray = p.scene.cameras_[p.activeCamera]->generateRay(xyPos.x, xyPos.y);
				for (int i = 0; i < p.scene.primitives.size(); ++i) {
					if (p.scene.primitives[i]->intersect(p.scene.cameras_[p.activeCamera]->position, ray)) {
						color = glm::vec4(1, 0, 0, 1);
					}
					else {
						color = glm::vec4(0, 1, 0, 1);
					}
				}
			}
			p.renderTexture_.setPixelColor(x, y, ImVec4(color.r, color.g, color.b, color.a));
		}
	}

	p.renderTexture_.updateTextureData();
}

void DebugRenderer::RenderPixel() {
	glm::vec2 xyPos;
	glm::vec3 ray;
	glm::vec4 color;
	for (int currentSample = 0; currentSample < p.samples_; ++currentSample) {
		xyPos = getSamplePosition(p.currentx, p.currenty, currentSample);
		ray = p.scene.cameras_[p.activeCamera]->generateRay(xyPos.x, xyPos.y);
		for (int i = 0; i < p.scene_.size(); ++i) {
			if (p.scene_[i]->intersect(p.scene.cameras_[p.activeCamera]->position, ray)) {
				color = glm::vec4(1, 0, 0, 1);
			}
			else {
				color = glm::vec4(0, 1, 0, 1);
			}
		}
	}

	p.renderTexture_.setPixelColor(p.currentx, p.currenty, ImVec4(color.r, color.g, color.b, color.a));
	p.renderTexture_.updateTextureData();
}

void DebugRenderer::setBackgroundColor(ImVec4 color) {
	p.renderTexture_.clearTextureColor(color);
}

glm::vec2 DebugRenderer::getSamplePosition(int x, int y, int currentSample)
{
	// Check correct calculations based aspectratio, fov etc. Is a pixel truly 1:1 in size? or w/h and h/w respectively
	glm::vec2 pos = glm::vec2(0);
	float width = p.scene.cameras_[p.activeCamera]->resolution.x;
	float height = p.scene.cameras_[p.activeCamera]->resolution.y;
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
		pos.x += r / width;
		pos.y += r / height;
	}
	return pos;
}
