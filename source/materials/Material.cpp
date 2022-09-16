#pragma once

#include <materials/Material.h>
#include <imgui.h>

Material::Material() : color_{ glm::vec3(1.0, 0.0, 0.0) }, reflectance_{ 0.0f }, transmittance_{ 0.0f }, ior_{ 1.33f }
{
}

Material::~Material()
{
}

Material::Material(const Material& old)
{
	color_ = old.color_;
	reflectance_ = old.reflectance_;
	transmittance_ = old.transmittance_;
	ior_ = old.ior_;
}

Material* Material::clone() const
{
	return new Material(*this);
}

void Material::GUI() {
	ImGui::ColorEdit3("Color", &color_.x);
	ImGui::SliderFloat("Reflectance", &reflectance_, 0.0f, 1.0f);
	ImGui::SliderFloat("Transmittance", &transmittance_, 0.0f, 1.0f);
	ImGui::SliderFloat("IOR", &ior_, 0.0f, 1.0f);
}
