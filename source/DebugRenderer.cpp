#pragma once

#include "DebugRenderer.h"
#include <Ray.h>

DebugRenderer::DebugRenderer(Parameters& params) : p{ params } {}

void DebugRenderer::Render(const Scene& scene) {
	size_t width = p.renderTexture_.getWidth();
	size_t height = p.renderTexture_.getHeight();
	glm::vec4 color = glm::vec4(0);
	glm::vec2 xyPos = glm::vec2(0);
	Ray ray;
	// Raster space [0, width] && [0, height]
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			for (int currentSample = 1; currentSample <= scene.cameras_[p.activeCamera]->samples; ++currentSample) {
				xyPos = p.samplers_[p.activeSampler]->getSamplePosition(x, y, *p.scene.cameras_[p.activeCamera], currentSample);
				ray = p.scene.cameras_[p.activeCamera]->generateRay(xyPos);
			
				for (int i = 0; i < p.scene.primitives.size(); ++i) {
					if (x == 1280 / 2 || y == 720 / 2) {
						color = glm::vec4(0, 0, 1, 1);
					}
					else if (p.scene.primitives[i]->intersect(ray)) {
						color = glm::vec4(1, 0, 0, 1);
					}
					else if (x == 0 || x == 1279 || y == 0 || y == 719) {
						color = glm::vec4(1, 1, 1, 1);
					}
					else {
						color = glm::vec4(0, 0, 0, 1);
					}
				}
				//average color here
			}
			p.renderTexture_.setPixelColor(x, y, ImVec4(color.r, color.g, color.b, color.a));
		}
	}
	p.renderTexture_.updateTextureData();
}

void DebugRenderer::GUISettings()
{
	ImGui::SameLine();
	ImGui::Text(": DebugRenderer");
}
