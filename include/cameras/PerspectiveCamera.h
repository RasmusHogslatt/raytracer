#pragma once

#include <cameras/Camera.h>

class PerspectiveCamera : public Camera {
public:
	PerspectiveCamera(float fov);
	Ray generateRay(const glm::vec2& samplePosition);
	void GUISettings();

	float vfov;
	float lensRadius;
};