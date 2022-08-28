#pragma once

#include <glm/glm.hpp>
#include <Ray.h>

class Camera {
public:
	Camera(glm::vec3 position = glm::vec3(0, 0, 8), glm::vec3 front = glm::vec3(0, 0, -1), glm::vec3 up = glm::vec3(0, 1, 0), glm::ivec2 resolution = glm::ivec2(1920, 1080), int samples = 1, float fov = 55);

	void updateVectors();

	virtual Ray generateRay(const glm::vec2& samplePosition) = 0;
	virtual void GUISettings();

	glm::ivec2 resolution;
	glm::vec3 position;
	glm::vec3 up;
	glm::vec3 worldUp;
	glm::vec3 front;
	glm::vec3 right;
	float yaw, pitch;
	int samples;
	float aspectRatio;
	float fov;
	float randomFloat();
	float randomFloat(float min, float max);
};