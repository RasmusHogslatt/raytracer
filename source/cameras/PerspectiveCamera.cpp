#pragma once

#include <cameras/PerspectiveCamera.h>
#include <imgui.h>

PerspectiveCamera::PerspectiveCamera() : Camera() {}

Ray PerspectiveCamera::generateRay(const glm::vec2& samplePosition)
{
	float width = static_cast<float>(resolution_.x);
	float height = static_cast<float>(resolution_.y);

	glm::vec3 lowerLeft = position_ + front_ - (right_ / width) - up_ / height;

	glm::vec3 planeSample = lowerLeft + right_ * samplePosition.x + up_ * samplePosition.y;

	glm::vec3 rayDirection = glm::normalize(planeSample - position_);

	Ray ray(position_, rayDirection);
	return ray;
}

void PerspectiveCamera::GUI()
{
	ImGui::SameLine();
	ImGui::Text(": Perspective Camera");
	Camera::GUI();
}