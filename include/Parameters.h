#pragma once

#include <GLFW/glfw3.h>
#include <Texture.h>

struct Parameters
{
	// Window
	GLFWwindow* glfwWindow_ = nullptr;
	int glfwWidth_ = 1920;
	int glfwHeight_ = 1080;
	const char* glfwTitle_ = "Raytracer";
	
	// Rendertexture
	Texture renderTexture_;

	// Viewporttexture
	Texture viewportTexture_;
};