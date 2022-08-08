#pragma once

#include <GLFW/glfw3.h>
#include <Texture.h>
#include <Renderer.h>
#include <Actor.h>
#include <vector>
#include <memory>
#include <Camera.h>

struct ViewportActor {
	ImVec2 position = ImVec2(0,0);
	float z = 0;
	float radius;
	ImVec4 color = ImVec4(0.5,0.5,0.5,1);
};

struct ViewportCamera {
	ImVec2 position = ImVec2(0,0);
	float z = 0;
	ImVec4 color = ImVec4(1.0,0.5,0.5,1);
};

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
	glm::vec3 actorPos_ = glm::vec3(0);
	Material material_ = Material();
	float radius_ = 1.0;

	std::vector <std::shared_ptr<Renderer>> renderers_;
	std::vector <std::shared_ptr<Actor>> scene_;
	bool showViewportActor;

	// Camera
	Camera camera = Camera();

	// Renderer
	int activeRenderer_ = 0;
	bool renderOnePixel_ = true;
	bool pauseRenderer_ = false;
	int samples_ = 1;
	bool renderStart = false;
	size_t currentx = 0;
	size_t currenty = 0;
	size_t textureSize = 768 * 480;
	std::vector<ViewportActor> viewportActors;
	int sampleMode = 0;
};