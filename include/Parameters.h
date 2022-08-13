#pragma once

#include <GLFW/glfw3.h>
#include <Texture.h>
#include <Actor.h>
#include <vector>
#include <memory>
#include <MyCamera.h>
#include <Scene.h>
#include <MyMaterial.h>
#include <integrators/Integrator.h>
#include <samplers/Sampler.h>

struct ViewportActor {
	ImVec2 position = ImVec2(0,0);
	float z = 0;
	float radius = 0;
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
	const char* glfwTitle_ = "Physical based raytracer";

	// Rendertexture
	Texture renderTexture_;

	// Viewporttexture
	Texture viewportTexture_;

	// Scene
	glm::vec3 actorPos_ = glm::vec3(0);
	MyMaterial material_ = MyMaterial();
	float radius_ = 1.0;

	Scene scene;
	std::vector <std::shared_ptr<Integrator>> renderers_;
	std::vector <std::shared_ptr<Sampler>> samplers_;
	bool showViewportActor;

	// Select methods
	int activeCamera = 0;
	int activeRenderer_ = 0;
	int activeSampler = 0;
	
	// Renderer
	int samples_ = 1;
	bool renderStart = false;
	size_t textureSize = 768 * 480;
	std::vector<ViewportActor> viewportActors;
	int sampleMode = 0;
};