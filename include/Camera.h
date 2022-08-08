#pragma once

#include <glm/glm.hpp>

class Camera {
public:
	Camera(glm::vec3 position = glm::vec3(0), glm::vec3 front = glm::vec3(0, 0, -1), glm::vec2 resolution = glm::vec2(1280, 720), float fov = 55.0, glm::vec3 up = glm::vec3(0, 1, 0));

	glm::vec3 generateRay(float x, float y);
	void updateVectors();

	float fov;
	glm::vec3 position;
	glm::ivec2 resolution;
	glm::vec3 direction;
	glm::vec3 up;
	glm::vec3 front;
	glm::vec3 right;
};