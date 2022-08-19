#pragma once

#include <App.h>
#include <GLFW/glfw3.h>
#include <Renderer.h>
#include <DebugRenderer.h>
#include <samplers/CenterSampler.h>
#include <samplers/Sampler.h>
#include <cameras/PerspectiveCamera.h>
#include <cameras/OrthographicCamera.h>
#include <samplers/RandomSampler.h>

void App::callGUI() {
	updateWindowSizeAndPosition();
	GUI::GUI(params);
}

void App::close() {
	glfwSetWindowShouldClose(params.glfwWindow_, true);
}

static void glfw_error_callback(int error, const char* description) {
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

App::App(int width, int height) {
	params.glfwWidth_ = width;
	params.glfwHeight_ = height;
	positions = std::vector<ImVec2>();
	initGLFWandIMGUI();

	params.renderTexture_.createTexture(1280, 720);
	params.viewportTexture_.createTexture();

	// Add one of each actor type for selection in GUI
	MySphere sphere = MySphere(1.0f);
	params.actors.push_back(&sphere);

	// Add custom cameras and renderers
	params.scene.cameras_.push_back(new PerspectiveCamera());
	params.scene.cameras_.push_back(new OrthographicCamera(45));

	// Add integrators
	params.renderers_.push_back(new DebugRenderer(params));
	
	// Add samplers
	params.samplers_.push_back(new CenterSampler());
	params.samplers_.push_back(new RandomSampler());
}

App::~App()
{
	std::cout << "Closing application. Goodbye!\n";

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(params.glfwWindow_);
	glfwTerminate();
}

void App::updateWindowSizeAndPosition() {
	glfwGetWindowSize(params.glfwWindow_, &params.glfwWidth_, &params.glfwHeight_);
	ImGui::SetNextWindowSize(ImVec2(static_cast<float>(params.glfwWidth_), static_cast<float>(params.glfwHeight_)));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
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
	params.glfwWindow_ = glfwCreateWindow(params.glfwWidth_, params.glfwHeight_, params.glfwTitle_, NULL, NULL);
	if (params.glfwWindow_ == NULL) {
		std::exit(1);
	}
	glfwMakeContextCurrent(params.glfwWindow_);
	glfwSwapInterval(1); // Enable vsync

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(params.glfwWindow_, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void App::loop() {
	while (!glfwWindowShouldClose(params.glfwWindow_)) {
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		callGUI();

		// Render ImGui
		ImGui::Render();
		glfwGetFramebufferSize(params.glfwWindow_, &params.glfwWidth_, &params.glfwHeight_);
		glViewport(0, 0, params.glfwWidth_, params.glfwHeight_);

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw all ImGui windows' DrawData
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(params.glfwWindow_);
	}
}
