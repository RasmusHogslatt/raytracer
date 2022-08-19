#pragma once

#include <cameras/Camera.h>
#include <random>

Camera::Camera(glm::vec3 position, glm::vec3 front, glm::vec3 up, glm::ivec2 resolution, int samples) : position{ position }, front{ front }, up{ up }, resolution{ resolution }, samples{ samples }, yaw{ -90.0f }, pitch{ 0.0f }, worldUp{ glm::vec3(0, 1, 0) }, aspectRatio{static_cast<float>(resolution.x) /  static_cast<float>(resolution.y)}
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

float Camera::randomFloat()
{
	return rand() / (RAND_MAX + 1.0f);
}

float Camera::randomFloat(float min, float max)
{
	return min + (max - min) * randomFloat();
}
