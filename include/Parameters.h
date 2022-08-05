#pragma once

#include <GLFW/glfw3.h>
#include <Texture.h>
#include <Renderer.h>
#include <Actor.h>
#include <vector>
#include <memory>

struct Parameters {
	// Window
	GLFWwindow* glfwWindow_ = nullptr;
	int glfwWidth_ = 1920;
	int glfwHeight_ = 1080;
	const char* glfwTitle_ = "Raytracer";

	// Rendertexture
	Texture renderTexture_;

	// Viewporttexture
	Texture viewportTexture_;

	// Scene
	float actorPos_[3] = { 0, 0, 0 };

	std::vector <std::shared_ptr<Renderer>> renderers_;
	std::vector <std::shared_ptr<Actor>> scene_;

	// Camera
	float cameraPos_[3] = { 0, 0, 0 };
	float cameraTarget_[3] = { 0, 0, 0 };
	float fov_ = 0;

	// Renderer
	int activeRenderer_ = 0;
	bool renderOnePixel_ = true;
	bool pauseRenderer_ = false;
	int samples_ = 1;
	bool renderStart = false;
	size_t currentx = 0;
	size_t currenty = 0;
	size_t textureSize = 768 * 480;
};