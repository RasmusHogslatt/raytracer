#pragma once

#include <cameras/PerspectiveCamera.h>
#include <imgui.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

PerspectiveCamera::PerspectiveCamera() : Camera(), lensRadius{ 1.0f }
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
	return ray;
}

void PerspectiveCamera::GUISettings()
{
	ImGui::SameLine();
	ImGui::Text(": Perspective Camera");
	Camera::GUISettings();
}
