#pragma once

#include <Renderer.h>
#include <Actor.h>
#include <vector>
#include <Texture.h>
#include <Parameters.h>
#include <Parameters.h>

class DebugRenderer : public Renderer {
public:
	DebugRenderer(Parameters& params);
	
	void Render();
	void RenderPixel();
	void setBackgroundColor(ImVec4 color = ImVec4(0.5, 0.5, 0.5, 1.0));
	glm::vec2 getSamplePosition(int x, int y, int currentSample);
private:
	Parameters& p;
};