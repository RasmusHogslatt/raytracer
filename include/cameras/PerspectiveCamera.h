#pragma once

#include <cameras/Camera.h>

class PerspectiveCamera : public Camera {
public:
	PerspectiveCamera();
	Ray generateRay(const glm::vec2& samplePosition);
	void GUISettings();

	float lensRadius;
};