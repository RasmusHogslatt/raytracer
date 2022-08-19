#pragma once

#include <cameras/OrthographicCamera.h>
#include <imgui.h>

OrthographicCamera::OrthographicCamera(float fov)
{
}

Ray OrthographicCamera::generateRay(const glm::vec2& samplePosition)
{
	glm::vec3 s = position + samplePosition.x * right/ 2.0f + samplePosition.y * up /2.0f;
	return Ray(s, front);
}

void OrthographicCamera::GUISettings()
{
	ImGui::SameLine();
	ImGui::Text(": Orthographic Camera");
	bool changed = false;
	changed = ImGui::SliderInt("Samples/Pixel", &samples, 1, 10);
	changed = ImGui::SliderFloat3("Position", &position.x, -10, 10);
	changed = ImGui::SliderFloat("Yaw", &yaw, -180, 180);
	changed = ImGui::SliderFloat("Pitch", &pitch, -180, 180);
	if (!changed) {
		updateVectors();
	}
}
