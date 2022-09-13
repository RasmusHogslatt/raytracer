 #pragma once

#include <core/Texture.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <cstdlib>
#include <memory>
#include <vector>
#include <functional>
#include <iostream>
#include <integrators/Integrator.h>
#include <glm/glm.hpp>
#include <core/Scene.h>

static void glfw_error_callback(int error, const char* description);

class App
{
public:
	App(int width = 2560, int height = 1440);
	~App();

	// Updates IMGUI window to full screen. Called each frame
	void updateWindowSizeAndPosition();

	void initGLFWandIMGUI();
	void loop();
	void close();

	void GUI();
	void GUIMenu();
	void GUIControlPanel();
	void GUIRenderTexture();
	void GUIViewports();

private:
	// GLFW window
	GLFWwindow* glfwWindow_;
	glm::ivec2 appResolution_;

	// Render window
	Texture renderTexture_;
	glm::ivec2 renderResolution_;
	bool renderStart_;

	Scene scene_;

	std::vector<Integrator*> integrators_;
	int activeIntegrator_;
};