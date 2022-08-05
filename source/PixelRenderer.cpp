#pragma once

#include "PixelRenderer.h"

PixelRenderer::PixelRenderer(Parameters& params) : p{ params } {
}

void PixelRenderer::Render() {

	setBackgroundColor(ImVec4(0, 0, 1, 1));


	p.renderTexture_.updateTextureData();
}

void PixelRenderer::RenderPixel() {
	int width = p.renderTexture_.getWidth();
	int height = p.renderTexture_.getHeight();
	p.renderTexture_.setPixelColor(p.currentx, p.currenty, ImVec4((float)p.currentx / width - 1, (float)p.currenty / height - 1, 0, 1));

	p.renderTexture_.updateTextureData();
}

void PixelRenderer::setBackgroundColor(ImVec4 color) {
	p.renderTexture_.clearTextureColor(color);
}
