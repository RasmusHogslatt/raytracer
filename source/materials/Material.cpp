#pragma once

#include <materials/Material.h>
#include <imgui.h>

Material::Material() : color_{ glm::vec3(1.0, 0.0, 0.0) }, reflectance_{ 1.0f }, transmittance_{ 0.0f }, ior_{ 1.33f }, name_{""}, id_{0}
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
	id_ = old.id_;
	name_ = old.name_;
}

//void Material::GUI() {
//	//ImGui::ColorEdit3("Color", &color_.x);
//	//ImGui::SliderFloat("Reflectance", &reflectance_, 0.0f, 1.0f);
//	//ImGui::SliderFloat("Transmittance", &transmittance_, 0.0f, 1.0f);
//	//ImGui::SliderFloat("IOR", &ior_, 0.0f, 1.0f);
//}


Lambertian::Lambertian() : Material()
{
	name_ = "Lambertian";
	reflectance_ = 1.0f;
	transmittance_ = 0.0f;
	id_ = 0;
}

Lambertian::~Lambertian()
{
}

Lambertian::Lambertian(const Lambertian& old)
{
	color_ = old.color_;
	reflectance_ = old.reflectance_;
	transmittance_ = old.transmittance_;
	ior_ = old.ior_;
	id_ = old.id_;
	name_ = old.name_;
}

Lambertian* Lambertian::clone() const
{
	return new Lambertian(*this);
}

void Lambertian::GUI()
{
	ImGui::Text(name_.c_str());
	ImGui::ColorEdit3("Color", &color_.x);
	ImGui::SliderFloat("Reflectance", &reflectance_, 0.0f, 1.0f);
}

float Lambertian::getFr()
{
	return reflectance_ / 3.14f;
}

Mirror::Mirror() : Material()
{
	name_ = "Mirror";
	reflectance_ = 1.0f;
	transmittance_ = 0.0f;
	id_ = 1;
}

Mirror::~Mirror()
{
}

Mirror::Mirror(const Mirror& old)
{
	color_ = old.color_;
	reflectance_ = old.reflectance_;
	transmittance_ = old.transmittance_;
	ior_ = old.ior_;
	id_ = old.id_;
	name_ = old.name_;
}

Mirror* Mirror::clone() const
{
	return new Mirror(*this);
}

void Mirror::GUI()
{
	ImGui::Text(name_.c_str());
}

float Mirror::getFr()
{
	return reflectance_;
}

Glass::Glass() : Material()
{
	name_ = "Glass";
	reflectance_ = 1.0f;
	transmittance_ = 1.0f;
	id_ = 2;
	ior_ = 1.5f;
}

Glass::~Glass()
{
}

Glass::Glass(const Glass& old)
{
	color_ = old.color_;
	reflectance_ = old.reflectance_;
	transmittance_ = old.transmittance_;
	ior_ = old.ior_;
	id_ = old.id_;
	name_ = old.name_;
}

Glass* Glass::clone() const
{
	return new Glass(*this);
}

void Glass::GUI()
{
	ImGui::Text(name_.c_str());
	ImGui::SliderFloat("IOR", &ior_, 0.1f, 4.33f);
}

float Glass::getFr()
{
	return reflectance_;
}
