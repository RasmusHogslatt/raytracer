#pragma once

#include <core/App.h>
#include <GLFW/glfw3.h>
#include <integrators/MonteCarloIntegrator.h>
#include <samplers/CenterSampler.h>
#include <samplers/Sampler.h>
#include <cameras/PerspectiveCamera.h>
#include <cameras/OrthographicCamera.h>
#include <samplers/RandomSampler.h>
#include <lights/Light.h>


void App::GUI()
{
	updateWindowSizeAndPosition();
	if (ImGui::Begin("MainWindow", 0, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize)) {
		GUIMenu();
		GUIControlPanel();
		GUIRenderTexture();
		GUIViewports();
	}
	ImGui::End();
}

void App::GUIMenu() {
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Close")) {
			glfwSetWindowShouldClose(glfwWindow_, true);
			if (ImGui::MenuItem("Close")) {
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Export PNG")) {
				renderTexture_.exportPNG("Output", 4);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

void App::GUIControlPanel() {
	ImGui::BeginChild("ControlPanel", ImVec2(600, 0), true);
	ImGui::SliderInt("Active integrator", &activeIntegrator_, 0, integrators_.size() - 1);
	if (ImGui::Button("RENDER")) {
		renderStart_ = true;
	}
	integrators_[activeIntegrator_]->GUI();
	scene_.GUI();
	ImGui::EndChild();
}

void App::GUIRenderTexture() {
	if (renderStart_) {
		integrators_[activeIntegrator_]->Integrate(renderTexture_);
		renderStart_ = false;
	}
	ImVec2 topleft = ImVec2(620, 25);
	ImVec2 bottomright = ImVec2(topleft.x + renderTexture_.getWidth(), topleft.y + renderTexture_.getHeight());
	ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)renderTexture_.ID_, topleft, bottomright);
	renderTexture_.updateTextureData();
}

void App::GUIViewports() {
	float yPos = 35.0f + static_cast<float>(renderResolution_.y);
	float xPos = 620.0f;
	auto drawList = ImGui::GetWindowDrawList();
	ImU32 color = ImGui::ColorConvertFloat4ToU32(ImVec4(1, 1, 1, 1));

	// XZ-window
	drawList->AddText(ImVec2(xPos + 5, yPos + 5), color, "XZ-plane");
	drawList->AddRect(ImVec2(xPos, yPos), ImVec2(xPos + 300, yPos + 300), color);
	xPos += 310;

	// XZ-window
	drawList->AddText(ImVec2(xPos + 5, yPos + 5), color, "ZY-plane");
	drawList->AddRect(ImVec2(xPos, yPos), ImVec2(xPos + 300, yPos + 300), color);
	xPos += 310;

	// XZ-window
	drawList->AddText(ImVec2(xPos + 5, yPos + 5), color, "XY-plane");
	drawList->AddRect(ImVec2(xPos, yPos), ImVec2(xPos + 300, yPos + 300), color);
}

static void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

App::App(int width, int height) : renderStart_{ false }, renderResolution_{ 1920, 1080 }, glfwWindow_{ nullptr }, appResolution_{ glm::ivec2(width, height) }, activeIntegrator_{ 0 } {
	initGLFWandIMGUI();

	renderTexture_.createTexture(renderResolution_.x, renderResolution_.y);

	integrators_.push_back(new MonteCarloIntegrator(&scene_));
}

App::~App()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(glfwWindow_);
	glfwTerminate();
}

void App::updateWindowSizeAndPosition() {
	ImGui::SetNextWindowSize(ImVec2(static_cast<float>(appResolution_.x), static_cast<float>(appResolution_.y)));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
}

void App::close() {
	glfwSetWindowShouldClose(glfwWindow_, true);
}

void App::initGLFWandIMGUI() {
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		std::exit(1);

	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	// Create window with graphics context
	auto monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	glfwWindow_ = glfwCreateWindow(mode->width, mode->height, "Monte Carlo Ray Tracer", monitor, NULL);
	if (glfwWindow_ == nullptr) {
		std::exit(1);
	}
	glfwMakeContextCurrent(glfwWindow_);
	glfwSwapInterval(1); // Enable vsync

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(glfwWindow_, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void App::loop() {
	while (!glfwWindowShouldClose(glfwWindow_)) {
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		GUI();

		// Render ImGui
		ImGui::Render();

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw all ImGui windows' DrawData
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(glfwWindow_);
	}
}
