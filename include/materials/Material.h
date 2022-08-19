#pragma once

#include <imgui.h>
#include <glm/glm.hpp>
#include <vector>

class Material {
public:
	Material();
	glm::vec4 diffuseColor = glm::vec4(0.5, 0, 0, 1);
	glm::vec4 specularColor = glm::vec4(0.5, 0, 0, 1);
	glm::vec4 ambientColor = glm::vec4(0.5, 0, 0, 1);
	glm::vec4 emissiveColor = glm::vec4(1, 1, 1, 1);
	bool isEmissive = false;
	float ior = 1.3f;
	std::vector<glm::vec4> colors = std::vector<glm::vec4>();
	void setDiffuseColor(glm::vec4 c);
	void setSpecularColor(glm::vec4 c);
	void setAmbientColor(glm::vec4 c);
	void setEmissiveColor(glm::vec4 e);
	void setEmissive(bool emissive);
	void setIOR(float IOR);

	void GUISettings();
	int colorType = 1;
};