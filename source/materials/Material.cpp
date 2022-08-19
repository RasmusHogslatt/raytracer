#pragma once

#include <materials/Material.h>

Material::Material()
{
	glm::vec4 dummyColor = glm::vec4(1,0,0,1);
	for (int i = 0; i < 4; ++i) {
		colors.push_back(dummyColor);
	}
}

void Material::setDiffuseColor(glm::vec4 c)
{
	diffuseColor = c;
}

void Material::setSpecularColor(glm::vec4 c)
{
	specularColor = c;
}

void Material::setAmbientColor(glm::vec4 c)
{
	ambientColor = c;
}

void Material::setEmissiveColor(glm::vec4 e)
{
	emissiveColor = e;
}

void Material::setEmissive(bool emissive)
{
	isEmissive = emissive;
}

void Material::setIOR(float IOR)
{
	ior = IOR;
}

void Material::GUISettings()
{
	ImGui::Text("MATERIAL PROPERTIES");
	ImGui::SliderInt("Color type", &colorType, 1, 4);
	ImGui::ColorPicker4("Pick color", &colors[colorType].x);
	ImGui::SliderFloat("IOR", &ior, 0, 4);
}
