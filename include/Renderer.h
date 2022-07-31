#pragma once

#include <Texture.h>
#include <vector>
#include <Object.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Renderer
{
public:
	Renderer(Texture& texture);
	/// <summary>
	/// Takes a scene and renders to image_ with colors in range [0,1]
	/// </summary>
	void Render();
	glm::vec3 getInitialRay();
	//glm::vec4 trace(glm::vec3& rayOrigo, glm::vec3& rayDirection, const int& depth);
	unsigned char* getImageTextureData() const;
private:
	uint32_t width_;
	uint32_t height_;
	Texture &image_;
	glm::vec3 cameraPosition_ = glm::vec3(0);
	float fov_ = 30.0f;
	int maxDepth_ = 3;
};