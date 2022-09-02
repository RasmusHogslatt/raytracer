#pragma once

#include <materials/Material.h>
#include <imgui.h>

Material::Material() : diffuse_{ glm::vec3(1.0, 0.0, 0.0) }, specular_{ glm::vec3(1.0, 0.0, 0.0) }
{
}

Material::~Material()
{
}

Material::Material(const Material& old)
{
	diffuse_ = old.diffuse_;
	specular_ = old.specular_;
}

void Material::GUI() {
	ImGui::ColorEdit3("Diffuse color", &diffuse_.x);
	ImGui::ColorEdit3("Specular color", &specular_.x);
}
