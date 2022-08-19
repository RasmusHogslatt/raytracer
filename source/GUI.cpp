#pragma once

#include "GUI.h"
#include <stdint.h>

#include <iostream>
#include <MySphere.h>
#include <cmath>

glm::vec4 ImVec4ToGLM(const ImVec4& v)
{
	return glm::vec4(v.x, v.y, v.z, v.w);
}

ImVec4 GLMToImVec4(const glm::vec4& v)
{
	return ImVec4(v.x, v.y, v.z, v.w);
}

GUI::GUI(Parameters& appParameters) : params{ appParameters }, renderTexturePos_{ 624, 30 } {
	viewportPos_ = ImVec2(renderTexturePos_.x, renderTexturePos_.y + params.renderTexture_.getHeight() + 10);
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
	misc();
	ImGui::NewLine();
	camerasettings();
	ImGui::NewLine();
	rendersettings();
	ImGui::NewLine();
	sceneconstruction();
	ImGui::EndChild();
}

void GUI::misc() {
	ImGui::SliderInt("Active renderer", &params.activeRenderer_, 0, static_cast<int>(params.renderers_.size() - 1));
	ImGui::SliderInt("Active Camera", &params.activeCamera, 0, static_cast<int>(params.scene.cameras_.size() - 1));
	ImGui::SliderInt("Active sampler", &params.activeSampler, 0, static_cast<int>(params.samplers_.size() - 1));
	ImGui::Checkbox("Show viewport actor", &params.showViewportActor);
}

void GUI::sceneconstruction() {
	ImGui::Text("SCENE OBJECT");
	ImGui::SliderInt("Select object type", &params.activeObjectSelection, 0, static_cast<int>(params.samplers_.size() - 1));
	if (ImGui::Button("Add actor")) {
		if (params.activeObjectSelection == 0) {
			params.scene.primitives.push_back(new MySphere(params.sphere));
		}
		else if (params.activeObjectSelection == 1) {
			//pushback other primitive
		}

		// To viewport
		dummyActor.position = ImVec2(params.actorPos_.x, params.actorPos_.y);
		dummyActor.z = params.actorPos_.z;
		dummyActor.radius = params.radius_;
		dummyActor.color = GLMToImVec4(params.material_.colors[params.material_.colorType]);
		params.viewportActors.push_back(dummyActor);
	}
	if (params.activeObjectSelection == 0) {
		params.sphere.GUISettings();
	}
	else if (params.activeObjectSelection == 1) {
		//guisettings for other primitive
	}

}

void GUI::camerasettings() {
	ImGui::Text("CAMERA");
	params.scene.cameras_[params.activeCamera]->GUISettings();
}

void GUI::rendersettings() {
	ImGui::Text("RENDERER");
	params.renderers_[params.activeRenderer_]->GUISettings();

	if (ImGui::Button("Render")) {
		params.renderStart = true;
	}
}

void GUI::viewportPanel() {
	ImGui::SameLine();
	ImGui::BeginChild("Viewports", ImVec2(0, 0), true);
	renderport();
	glviewport();
	ImGui::EndChild();
}

void GUI::renderport() {
	if (params.renderStart) {
		params.renderers_[params.activeRenderer_]->Render(params.scene);
		params.renderStart = false;
	}
	ImVec2 topleft = renderTexturePos_;
	ImVec2 bottomright = ImVec2(topleft.x + params.renderTexture_.getWidth(), topleft.y + params.renderTexture_.getHeight());
	ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)params.renderTexture_.ID_, topleft, bottomright);
	
	params.renderTexture_.updateTextureData();
}


void GUI::glviewport()
{
	// Viewport dimensions
	ImVec2 dims = ImVec2(300, 300);
	int scale = 15;
	int margin = 10;
	float cameraRadius = 4.0;
	ImVec2 secondWindowPos = ImVec2(viewportPos_.x + dims.x + margin, viewportPos_.y);
	ImVec2 thirdWindowPos = ImVec2(secondWindowPos.x + dims.x + margin, secondWindowPos.y);
	ImVec2 centerOfViewport1 = ImVec2(viewportPos_.x + dims.x / 2, viewportPos_.y + dims.y / 2);
	ImVec2 centerOfViewport2 = ImVec2(secondWindowPos.x + dims.x / 2, secondWindowPos.y + dims.y / 2);
	ImVec2 centerOfViewport3 = ImVec2(thirdWindowPos.x + dims.x / 2, thirdWindowPos.y + dims.y / 2);
	ImU32 textColor = ImGui::ColorConvertFloat4ToU32(ImVec4(1, 1, 1, 1));
	ImU32 borderColor = ImGui::ColorConvertFloat4ToU32(ImVec4(1, 1, 1, 1));
	ImU32 dummyColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0.1f, 0.3f, 1.0f, 1.0f));

	// 1st viewport
	auto drawList = ImGui::GetWindowDrawList();
	drawList->AddText(viewportPos_, textColor, "XZ-plane");
	drawList->AddRect(viewportPos_, ImVec2(viewportPos_.x + dims.x, viewportPos_.y + dims.y), borderColor);

	// Camera
	ImVec2 cam1pos = ImVec2(centerOfViewport1.x + params.scene.cameras_[params.activeCamera]->position.x * scale, centerOfViewport1.y + params.scene.cameras_[params.activeCamera]->position.z * scale);
	ImVec2 cam1dir = ImVec2(centerOfViewport1.x + (params.scene.cameras_[params.activeCamera]->position.x + params.scene.cameras_[params.activeCamera]->front.x) * scale, centerOfViewport1.y + (params.scene.cameras_[params.activeCamera]->position.z + params.scene.cameras_[params.activeCamera]->front.z) * scale);
	drawList->AddCircleFilled(cam1pos, cameraRadius, textColor);
	drawList->AddLine(cam1pos, cam1dir, textColor);

	if (params.showViewportActor) {
		drawList->AddCircleFilled(ImVec2(centerOfViewport1.x + params.sphere.position_.x * scale, centerOfViewport1.y + params.sphere.position_.z * scale), params.sphere.radius_ * scale, dummyColor);
	}
	for (int i = 0; i < params.scene.primitives.size(); ++i) {
		drawList->AddCircleFilled(ImVec2(centerOfViewport1.x + params.scene.primitives[i]->position_.x * scale, centerOfViewport1.y + params.scene.primitives[i]->position_.z * scale), params.viewportActors[i].radius * scale, ImGui::ColorConvertFloat4ToU32(params.viewportActors[i].color));
	}

	// 2nd viewport
	drawList->AddText(secondWindowPos, textColor, "ZY-plane");
	drawList->AddRect(secondWindowPos, ImVec2(secondWindowPos.x + dims.x, secondWindowPos.y + dims.y), borderColor);

	// Camera
	ImVec2 cam2pos = ImVec2(centerOfViewport2.x - params.scene.cameras_[params.activeCamera]->position.z * scale, centerOfViewport2.y - params.scene.cameras_[params.activeCamera]->position.y * scale);
	ImVec2 cam2dir = ImVec2(centerOfViewport2.x - (params.scene.cameras_[params.activeCamera]->position.z + params.scene.cameras_[params.activeCamera]->front.z) * scale, centerOfViewport2.y - (params.scene.cameras_[params.activeCamera]->position.y + params.scene.cameras_[params.activeCamera]->front.y) * scale);
	drawList->AddCircleFilled(cam2pos, cameraRadius, textColor);
	drawList->AddLine(cam2pos, cam2dir, textColor);

	if (params.showViewportActor) {
		drawList->AddCircleFilled(ImVec2(centerOfViewport2.x - params.sphere.position_.z * scale, centerOfViewport2.y - params.sphere.position_.y * scale), params.sphere.radius_ * scale, dummyColor);
	}
	for (int i = 0; i < params.viewportActors.size(); ++i) {
		drawList->AddCircleFilled(ImVec2(centerOfViewport2.x - params.viewportActors[i].z * scale, centerOfViewport2.y - params.viewportActors[i].position.y * scale), params.viewportActors[i].radius * scale, ImGui::ColorConvertFloat4ToU32(params.viewportActors[i].color));
	}

	// 3rd viewport
	drawList->AddText(thirdWindowPos, textColor, "XY-plane");
	drawList->AddRect(thirdWindowPos, ImVec2(thirdWindowPos.x + dims.x, thirdWindowPos.y + dims.y), borderColor);

	// Camera
	ImVec2 cam3pos = ImVec2(centerOfViewport3.x + params.scene.cameras_[params.activeCamera]->position.x * scale, centerOfViewport3.y - params.scene.cameras_[params.activeCamera]->position.y * scale);
	ImVec2 cam3dir = ImVec2(centerOfViewport3.x + (params.scene.cameras_[params.activeCamera]->position.x + params.scene.cameras_[params.activeCamera]->front.x) * scale, centerOfViewport3.y - (params.scene.cameras_[params.activeCamera]->position.y + params.scene.cameras_[params.activeCamera]->front.y) * scale);
	drawList->AddCircleFilled(cam3pos, cameraRadius, textColor);
	drawList->AddLine(cam3pos, cam3dir, textColor);

	if (params.showViewportActor) {
		drawList->AddCircleFilled(ImVec2(centerOfViewport3.x + params.sphere.position_.x * scale, centerOfViewport3.y - params.sphere.position_.y * scale), params.sphere.radius_ * scale, dummyColor);
	}
	for (int i = 0; i < params.viewportActors.size(); ++i) {
		drawList->AddCircleFilled(ImVec2(centerOfViewport3.x + params.viewportActors[i].position.x * scale, centerOfViewport3.y - params.viewportActors[i].position.y * scale), params.viewportActors[i].radius * scale, ImGui::ColorConvertFloat4ToU32(params.viewportActors[i].color));
	}
}

