#pragma once

#include <cameras/Camera.h>
#include <random>
#include <imgui.h>

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, glm::ivec2 resolution, int samples, float fov) : position{ position }, front{ front }, up{ up }, resolution{ resolution }, samples{ samples }, yaw{ -90.0f }, pitch{ 0.0f }, worldUp{ glm::vec3(0, 1, 0) }, aspectRatio{ static_cast<float>(resolution.x) / static_cast<float>(resolution.y) }, fov{ fov }
{
	updateVectors();
}

void Camera::updateVectors()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::GUISettings()
{
	int changed = 0;
	changed += ImGui::SliderInt("Samples/Pixel", &samples, 1, 10);
	changed += ImGui::SliderFloat3("Position", &position.x, -10, 10);
	changed += ImGui::SliderFloat("Yaw", &yaw, -180, 180);
	changed += ImGui::SliderFloat("Pitch", &pitch, -180, 180);
	changed += ImGui::SliderFloat("FOV", &fov, 10, 150);
	if (changed != 0) {
		updateVectors();
	}
}

float Camera::randomFloat()
{
	return rand() / (RAND_MAX + 1.0f);
}

float Camera::randomFloat(float min, float max)
{
	return min + (max - min) * randomFloat();
}
