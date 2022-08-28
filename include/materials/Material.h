#pragma once

#include <imgui.h>
#include <glm/glm.hpp>
#include <vector>

// Reflection constant = Ks = colors[2].a
class Material {
public:
	Material();
	bool isEmissive = false;
	float ior = 1.3f;
	float shininess = 10.0f;
	std::vector<glm::vec4> colors = std::vector<glm::vec4>();
	void setDiffuseColor(glm::vec4 c);
	void setSpecularColor(glm::vec3 c);
	void setAmbientColor(glm::vec4 c);
	void setEmissiveColor(glm::vec4 e);
	void setEmissive(bool emissive);
	void setIOR(float IOR);

	glm::vec4 getAmbient();
	glm::vec4 getDiffuse();
	glm::vec4 getSpecular();
	glm::vec4 getEmissive();

	glm::vec3 getDiffuseRGB();
	glm::vec3 getSpecularRGB();
	glm::vec3 getEmissiveRGB();
	float getDiffuseStrength();
	float getSpecularStrength();
	float getEmissiveStrength();

	void GUISettings();
	int colorType = 1;
};