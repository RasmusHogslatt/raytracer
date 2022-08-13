#pragma once

#include <MyCamera.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

MyCamera::MyCamera(glm::vec3 position, glm::vec3 front, glm::vec2 resolution, float fov, glm::vec3 up, Texture* texture) : position{ position }, fov{ fov }, resolution{ resolution }, front{ front }, direction{front}, up{up}, texture{texture}
{
	updateVectors();
}

void MyCamera::setTexture(Texture& t)
{
	texture = &t;
}

glm::vec3 MyCamera::generateRay(float x, float y)
{
	glm::vec3 imagePoint = x * right - y * up + position + direction;
	glm::vec3 rayDirection = glm::normalize(imagePoint - position);
	return rayDirection;
}

void MyCamera::updateVectors()
{
	front = glm::normalize(front);
	direction = glm::normalize(direction);
	right = glm::normalize(glm::cross(direction, up));
	up = glm::normalize(glm::cross(right, direction));
}

void MyCamera::GUIsettings()
{
	ImGui::BeginChild("Settings");
	bool changed = false;
	changed = ImGui::SliderFloat3("Camera position", &position.x, -10, 10);
	changed = ImGui::SliderFloat3("Camera direction", &direction.x, -1, 1);
	changed = ImGui::SliderFloat("FOV", &fov, 10, 150);
	if (!changed) {
		updateVectors();
	}
	ImGui::EndChild();
}
