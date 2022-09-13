#pragma once

#include <materials/Material.h>
#include <imgui.h>

Material::Material() : color_{ glm::vec3(1.0, 0.0, 0.0) }, reflectance_{ 0.0f }
{
}

Material::~Material()
{
}

Material::Material(const Material& old)
{
	color_ = old.color_;
	reflectance_ = old.reflectance_;
}

Material* Material::clone() const
{
	return new Material(*this);
}

void Material::GUI() {
	ImGui::ColorEdit3("Color", &color_.x);
	ImGui::SliderFloat("Reflectance", &reflectance_, 0.0f, 1.0f);
}
