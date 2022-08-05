#pragma once

#include "GUI.h"
#include <stdint.h>

#include <iostream>
#include <Sphere.h>
#include <cmath>

GUI::GUI(Parameters& appParameters) : params{ appParameters }, renderTexturePos_{630, 50} {
	if (ImGui::Begin("MainWindow", 0, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize)) {
		menu();
		controlPanel();
		viewportPanel();
	}
	ImGui::End();
}

void GUI::menu() {
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Close")) {
				glfwSetWindowShouldClose(params.glfwWindow_, true);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
}

void GUI::controlPanel() {
	ImGui::BeginChild("Settings", ImVec2(600, 0), true);
	sceneconstruction();
	camerasettings();
	rendersettings();
	ImGui::EndChild();
}

void GUI::camerasettings() {
	ImGui::SliderFloat3("Camera position", params.cameraPos_, -10, 10);
	ImGui::SliderFloat3("Camera target", params.cameraTarget_, -10, 10);
	ImGui::SliderFloat("Field of view", &params.fov_, 20, 150);
}

void GUI::sceneconstruction() {
	ImGui::SliderFloat3("Actor position", params.actorPos_, -10, 10);
	if (ImGui::Button("Add actor")) {
		params.scene_.push_back(std::make_shared<Sphere>(Sphere()));
	std::cout << "Currently " << params.scene_.size() << " spheres added.\n";
	}
}

void GUI::rendersettings() {
	ImGui::SliderInt("Samples per pixel", &params.samples_, 1, 10);
	ImGui::SliderInt("Active renderer", &params.activeRenderer_, 0, static_cast<int>(params.renderers_.size() - 1));

	if (ImGui::Button("Render")) {
		params.currentx = 0;
		params.currenty = 0;
		params.renderStart = true;
		params.renderers_[params.activeRenderer_]->setBackgroundColor();
	}

	ImGui::SameLine();
	if (ImGui::Checkbox("Render one pixel", &params.renderOnePixel_)) {
		params.renderStart = false;
		params.renderers_[params.activeRenderer_]->setBackgroundColor();
	}

	ImGui::SameLine();
	if (ImGui::Checkbox("Pause", &params.pauseRenderer_)) {}

}

void GUI::viewportPanel() {
	ImGui::SameLine();
	ImGui::BeginChild("Viewports", ImVec2(0, 0), true);
	renderport();
	glviewport();
	ImGui::EndChild();
}

void GUI::renderport() {
	
	if (params.renderStart && !params.pauseRenderer_ && params.renderOnePixel_) {
		params.renderers_[params.activeRenderer_]->RenderPixel();
		if (params.currentx * params.currenty < params.textureSize) {
			if (params.currentx < params.renderTexture_.getWidth()) {
				params.currentx++;
			}
			else if (params.currentx == params.renderTexture_.getWidth()) {
				params.currentx = 0;
				params.currenty++;
			}
			else if (params.currentx * params.currenty == params.textureSize) {
				params.renderStart = false;
			}
		}
	}
	else if (params.renderStart && !params.renderOnePixel_) {
		params.renderers_[params.activeRenderer_]->Render();
		params.renderStart = false;
	}
	ImVec2 topleft = renderTexturePos_;
	ImVec2 bottomright = ImVec2(topleft.x + params.renderTexture_.getWidth(), topleft.y + params.renderTexture_.getHeight());
	ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)params.renderTexture_.ID_, topleft, bottomright);
	params.renderTexture_.updateTextureData();
}

void GUI::glviewport()
{
	ImVec2 renderportdims = params.renderTexture_.getDimensions();
	ImGui::GetWindowDrawList()->AddRect(ImVec2(renderTexturePos_.x, 800), ImVec2(renderTexturePos_.x + renderportdims.y, 1000), ImGui::ColorConvertFloat4ToU32(ImVec4(1, 0, 0, 1)));
}

