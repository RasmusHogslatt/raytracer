#pragma once

#include <App.h>
#include <GLFW/glfw3.h>
#include <Renderer.h>

void App::startUp()
{
	std::cout << "Welcome to Gamma!\n";
	params.renderTexture_.createTexture(ImVec4(1, 0, 0, 1));
	params.viewportTexture_.createTexture(ImVec4(1, 0, 0, 1));

	Renderer renderer(params.renderTexture_);
	renderer.Render();
	params.renderTexture_.setData(renderer.getImageTextureData());
	params.renderTexture_.updateTextureData();
}



void App::renderUI()
{
	updateWindowSizeAndPosition();
	GUI();
}

void App::close()
{
	glfwSetWindowShouldClose(params.glfwWindow_, true);
}

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void App::GUI()
{
	if (ImGui::Begin("Example: Simple layout", 0, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize))
	{

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Close"))
				{
					close();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		
		// Left pane properties
		{
			ImGui::BeginGroup();
			ImGui::BeginChild("LeftProperties", ImVec2(300, 0), true);

			ImGui::SliderFloat("X", &pos.x, 0.0f, 768);
			ImGui::SliderFloat("Y", &pos.y, 0.0f, 480);

			if (ImGui::Button("Render scene"))
			{
				x += 1;
				std::cout << "Raytracing\n";
			}
			ImGui::Text("Create scene");
			if (ImGui::Button("Add sphere"))
			{
				std::cout << "Value is: " << x << "\n";
				positions.push_back(pos);
			}


			ImGui::EndChild();
			ImGui::EndGroup();
		}

		ImGui::SameLine();

		// Viewport
		{
			ImGui::BeginGroup();
			ImVec2 viewportSize = ImVec2(768, 480);
			ImGui::BeginChild("Viewport", viewportSize);
			ImVec2 viewPos = ImGui::GetWindowPos();
			ImVec2 viewSize = ImGui::GetWindowSize();
			ImVec2 maxPos = ImVec2(viewPos.x + viewSize.x, viewPos.y + viewSize.y);
			
			ImVec2 currPos = ImVec2(viewPos.x + pos.x, viewPos.y + pos.y);
			
			ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)params.viewportTexture_.ID_, viewPos, maxPos);
			ImGui::GetWindowDrawList()->AddCircleFilled(currPos, 5.0f, ImColor(0, 255, 0, 255));
			for (uint32_t i = 0; i < positions.size(); ++i)
			{
				ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(positions[i].x + viewPos.x, positions[i].y + viewPos.y), 5.0f, ImColor(0, 0, 255, 255));
			}
			GUI::GUI(params);

			ImGui::EndChild();
			ImGui::EndGroup();
		}
		// Raytrace
		ImGui::SameLine();
		{
			ImGui::BeginGroup();
			ImVec2 viewportSize = ImVec2(768, 480);
			ImGui::BeginChild("Render", viewportSize);
			ImVec2 viewPos = ImGui::GetWindowPos();
			ImVec2 viewSize = ImGui::GetWindowSize();
			ImVec2 maxPos = ImVec2(viewPos.x + viewSize.x, viewPos.y + viewSize.y);
			ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)params.renderTexture_.ID_, viewPos, maxPos);
			ImGui::EndChild();
			ImGui::EndGroup();
		}
	}
	ImGui::End();
}

App::App(int width, int height)
{
	positions = std::vector<ImVec2>();
	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		std::exit(1);

	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);


	// Create window with graphics context
	params.glfwWindow_ = glfwCreateWindow(params.glfwWidth_, params.glfwHeight_, params.glfwTitle_, NULL, NULL);
	if (params.glfwWindow_ == NULL)
	{
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

App::~App()
{
	std::cout << "Closing application. Goodbye!\n";

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(params.glfwWindow_);
	glfwTerminate();
}

void App::updateWindowSizeAndPosition()
{
	glfwGetWindowSize(params.glfwWindow_, &params.glfwWidth_, &params.glfwHeight_);
	ImGui::SetNextWindowSize(ImVec2(static_cast<float>(params.glfwWidth_), static_cast<float>(params.glfwHeight_)));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
}

void App::run()
{
	startUp();

	while (!glfwWindowShouldClose(params.glfwWindow_))
	{
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		renderUI();
		
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

	close();
}
