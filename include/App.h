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

static void glfw_error_callback(int error, const char* description);

class App
{
public:
	App(int width = 1920, int height = 1080);

	~App();

	void updateWindowSizeAndPosition();

	void run();
	void startUp();
	void renderUI();

	void close();

	void GUI();

private:
	Parameters params;
	
	int x = 0;
	ImVec2 pos = ImVec2(0,0);
	std::vector<ImVec2> positions; // Construct objects from positions
	friend class GUI;
};