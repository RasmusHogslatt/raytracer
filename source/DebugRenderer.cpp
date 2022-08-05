#pragma once

#include "DebugRenderer.h"

DebugRenderer::DebugRenderer(Parameters& params) : p{ params } {}

void DebugRenderer::Render() {
	int width = p.renderTexture_.getWidth();
	int height = p.renderTexture_.getHeight();

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			p.renderTexture_.setPixelColor(x, y, ImVec4((float)x / width - 1, (float)y / width - 1, 0, 1));
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
