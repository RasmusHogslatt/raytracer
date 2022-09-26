#pragma once

#include <cameras/Camera.h>
#include <imgui.h>

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, glm::ivec2 resolution, int samples, float fov) : position_{ position }, front_{ front }, up_{ up }, resolution_{ resolution }, yaw_{ -90.0f }, pitch_{ 0.0f }, worldUp_{ glm::vec3(0, 1, 0) }, aspectRatio_{ static_cast<float>(resolution.x) / static_cast<float>(resolution.y) }, fov_{ fov }
{
	updateVectors();
}

void Camera::updateVectors()
{
	front_.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	front_.y = sin(glm::radians(pitch_));
	front_.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
	front_ = glm::normalize(front_);
	right_ = glm::normalize(glm::cross(front_, worldUp_));
	up_ = glm::normalize(glm::cross(right_, front_));
}

void Camera::GUI()
{
	int changed = 0;
	changed += ImGui::SliderFloat3("Camera position", &position_.x, -10, 10);
	changed += ImGui::SliderFloat("Yaw", &yaw_, -180, 180);
	changed += ImGui::SliderFloat("Pitch", &pitch_, -180, 180);
	changed += ImGui::SliderFloat("FOV", &fov_, 10, 150);
	if (changed != 0) {
		updateVectors();
	}
}