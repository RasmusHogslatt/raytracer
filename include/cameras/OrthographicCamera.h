#pragma once

#include <cameras/Camera.h>

class OrthographicCamera : public Camera {
public:
	OrthographicCamera(float fov);
	Ray generateRay(const glm::vec2& samplePosition);
	void GUISettings();
};