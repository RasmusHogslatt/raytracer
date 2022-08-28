#pragma once

#include <materials/Material.h>

Material::Material()
{
	glm::vec4 dummyColor = glm::vec4(0.5, 0.5, 0.5, 0.5);
	for (int i = 0; i < 4; ++i) {
		colors.push_back(dummyColor);
	}
}

void Material::setDiffuseColor(glm::vec4 c)
{
	colors[1] = c;
}

void Material::setSpecularColor(glm::vec3 c)
{
	colors[2].r = c.r;
	colors[2].g = c.g;
	colors[2].b = c.b;
}

void Material::setAmbientColor(glm::vec4 c)
{
	colors[0] = c;
}

void Material::setEmissiveColor(glm::vec4 e)
{
	colors[3] = e;
}

void Material::setEmissive(bool emissive)
{
	isEmissive = emissive;
}

void Material::setIOR(float IOR)
{
	ior = IOR;
}

glm::vec4 Material::getAmbient()
{
	return colors[0];
}

glm::vec4 Material::getDiffuse()
{
	return colors[1];
}

glm::vec4 Material::getSpecular()
{
	return colors[2];
}

glm::vec4 Material::getEmissive()
{
	return colors[4];
}

glm::vec3 Material::getDiffuseRGB()
{
	return glm::vec3(getDiffuse().r, getDiffuse().g, getDiffuse().b);
}

glm::vec3 Material::getSpecularRGB()
{
	return glm::vec3(getSpecular().r, getSpecular().g, getSpecular().b);
}

glm::vec3 Material::getEmissiveRGB()
{
	return glm::vec3(getEmissive().r, getEmissive().g, getEmissive().b);
}

float Material::getDiffuseStrength()
{
	return getDiffuse().a;
}

float Material::getSpecularStrength()
{
	return getSpecular().a;
}

float Material::getEmissiveStrength()
{
	return getEmissive().a;
}

void Material::GUISettings()
{
	ImGui::Text("MATERIAL PROPERTIES");
	ImGui::SliderFloat("Shininess", &shininess, 1.0f, 500.0f);
	ImGui::SliderFloat("Reflection/Specular Constant", &colors[2].a, 0.0f, 1.0f);
	ImGui::SliderInt("Color type : ", &colorType, 0, 3);
	ImGui::SameLine();
	if (colorType == 0) {
		ImGui::Text("Ambient");
	}
	else if (colorType == 1) {
		ImGui::Text("Diffuse");
	}
	else if (colorType == 2) {
		ImGui::Text("Specular");
	}
	else if (colorType == 3) {
		ImGui::Text("Emissive");
	}
	ImGui::ColorPicker4("Pick color", &colors[colorType].x);
	setSpecularColor(getDiffuseRGB());
	ImGui::SliderFloat("IOR", &ior, 0, 4);
	ImGui::Checkbox("Emissive", &isEmissive);
}
