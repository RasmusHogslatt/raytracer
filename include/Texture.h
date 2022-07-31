#pragma once

#include <stdint.h>
#include <glfw/glfw3.h>
#include <imgui.h>

class Texture
{
public:
	Texture();
	~Texture();
	void setClearColor(const ImVec4& color = ImVec4(1, 0, 0, 1));

	void setPixelColor(int x, int y, ImVec4 color);

	void clearTextureColor();

	void createTexture(const ImVec4& color, uint32_t width = 768, uint32_t height = 480);

	void updateTextureData();

	uint32_t getWidth() const;
	uint32_t getHeight() const;
	void setData(unsigned char* data);

	GLuint ID_;
	unsigned char* data_;
	ImVec4 clearColor_;
	ImVec2 getDimensions() const;
	bool isColorSet = false;
private:
	uint32_t width_;
	uint32_t height_;
	GLenum format_;
};