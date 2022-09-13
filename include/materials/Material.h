#pragma once

#include <glm/glm.hpp>

class Material {
public:
	Material();
	~Material();
	Material(const Material& old);
	Material* clone() const;
	void GUI();
public:
	glm::vec3 color_;
	float reflectance_;
};