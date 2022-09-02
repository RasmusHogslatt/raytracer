#pragma once

#include <glm/glm.hpp>

class Material {
public:
	Material();
	~Material();
	Material(const Material& old);
	void GUI();
public:
	glm::vec3 diffuse_;
	glm::vec3 specular_;
};