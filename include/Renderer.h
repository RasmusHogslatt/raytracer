#pragma once

#include <Texture.h>
#include <vector>
#include <Object.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Renderer {
public:
	virtual void Render() = 0;
	virtual void RenderPixel() = 0;
	virtual void setBackgroundColor(ImVec4 color = ImVec4(0.5, 0.5, 0.5, 1.0)) = 0;
private:
};