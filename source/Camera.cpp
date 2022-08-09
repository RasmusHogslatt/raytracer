#pragma once

#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec2 resolution, float fov, glm::vec3 up) : position{ position }, fov{ fov }, resolution{ resolution }, front{ front }, direction{front}, up{up}
{
	updateVectors();
}

glm::vec3 Camera::generateRay(float x, float y)
{

	glm::vec3 imagePoint = x * right - y * up + position + direction;
	glm::vec3 rayDirection = glm::normalize(imagePoint - position);
	return rayDirection;
}

void Camera::updateVectors()
{
	front = glm::normalize(front);
	direction = glm::normalize(direction);
	right = glm::normalize(glm::cross(direction, up));
	up = glm::normalize(glm::cross(right, direction));
}