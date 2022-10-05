#pragma once

#include <lights/RectangleLight.h>
#include <imgui.h>

RectangleLight::RectangleLight() : Light(new Rectangle())
{
}

RectangleLight::RectangleLight(const RectangleLight& old)
{
	shape_ = old.shape_;
	flux_ = old.flux_;
}

RectangleLight::~RectangleLight()
{
	delete shape_;
	shape_ = nullptr;
}

void RectangleLight::GUI()
{
	ImGui::Text("¨RectangleLight");
	Light::GUI();
	shape_->GUI();
}

float RectangleLight::getRadiosity(int samples)
{
	return flux_ / (shape_->getArea() / samples);
}
