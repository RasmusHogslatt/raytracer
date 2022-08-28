 #pragma once

#include <Texture.h>
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
#include <Parameters.h>
#include <GUI.h>
#include <DebugRenderer.h>

static void glfw_error_callback(int error, const char* description);

class App
{
public:
	App(int width = 2560, int height = 1440);

	~App();

	// Updates IMGUI window size. Called each frame
	void updateWindowSizeAndPosition();

	void initGLFWandIMGUI();
	void loop();
	void callGUI();

	void close();

	//void GUI();

private:
	Parameters params;
	std::vector<ImVec2> positions; // Construct objects from positions
};