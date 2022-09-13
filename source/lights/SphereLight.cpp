#pragma once

#include <lights/SphereLight.h>
#include <imgui.h>

SphereLight::SphereLight() : Light(new Sphere())
{
}

SphereLight::SphereLight(const SphereLight& old)
{
	shape_ = old.shape_;
	flux_ = old.flux_;
}

SphereLight::~SphereLight()
{
	delete shape_;
	shape_ = nullptr;
}

void SphereLight::GUI()
{
	ImGui::Text("SphereLight");
	Light::GUI();
	shape_->GUI();
}

float SphereLight::getRadiosity()
{
	return flux_ / shape_->getArea();
}
