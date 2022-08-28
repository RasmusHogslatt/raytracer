#pragma once

#include <Actor.h>

struct LightProperties {
	glm::vec3 color = glm::vec3(1,1,1);
	float intensity = 1.0f;
};

class Light : public Actor {
public:
	Light();
	Light(const Light& old);

	void GUISettings();

public:
	LightProperties lightProperties_;
};