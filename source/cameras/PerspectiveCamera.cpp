#pragma once

#include <cameras/PerspectiveCamera.h>
#include <imgui.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

PerspectiveCamera::PerspectiveCamera(float fov) : Camera(), vfov{ fov }, lensRadius{ 1.0f }
{
}

Ray PerspectiveCamera::generateRay(const glm::vec2& samplePosition)
{
	float width = resolution.x;
	float height = resolution.y;

	glm::vec3 lowerLeft = position + front - (right/width) - up/height;
	
	glm::vec3 planeSample = lowerLeft + right * samplePosition.x + up * samplePosition.y;

	glm::vec3 rayDirection = glm::normalize(planeSample - position);

	Ray ray(position, rayDirection);
	// TODO take fov into account
	return ray;
}

void PerspectiveCamera::GUISettings()
{
	ImGui::SameLine();
	ImGui::Text(": Perspective Camera");
	bool changed = false;
	changed = ImGui::SliderInt("Samples/Pixel", &samples, 1, 10);
	changed = ImGui::SliderFloat3("Position", &position.x, -10, 10);
	changed = ImGui::SliderFloat("Yaw", &yaw, -180, 180);
	changed = ImGui::SliderFloat("Pitch", &pitch, -180, 180);
	changed = ImGui::SliderFloat("FOV", &vfov, 10, 150);
	changed = ImGui::SliderFloat("Lens radius", &lensRadius, 0, 100);

	updateVectors();
}
