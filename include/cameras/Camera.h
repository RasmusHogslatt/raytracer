#pragma once

#include <core/Ray.h>

class Camera {
public:
	Camera(glm::vec3 position = glm::vec3(1, 0, 8), glm::vec3 front = glm::vec3(0, 0, -1), glm::vec3 up = glm::vec3(0, 1, 0), glm::ivec2 resolution = glm::ivec2(1920, 1080), float fov = 55.0f);

	void updateVectors();

	virtual Ray generateRay(const glm::vec2& samplePosition) = 0;
	virtual void GUI();

	glm::ivec2 resolution_;
	glm::vec3 position_;
	glm::vec3 up_;
	glm::vec3 worldUp_;
	glm::vec3 front_;
	glm::vec3 right_;
	float yaw_, pitch_;
	float aspectRatio_;
	float fov_;
};