#pragma once

#include <lights/Light.h>
#include <imgui.h>

Light::Light(Shape* shape) : shape_{ shape }, flux_{ 1.0f } {}

Light::Light(const Light& old) {
	shape_ = old.shape_->clone();
	flux_ = old.flux_;
}

Light* Light::clone() const
{
	return new Light(*this);
}

Light::~Light()
{
	
	shape_ = nullptr;
}

void Light::GUI()
{
	ImGui::NewLine();
	ImGui::Text("LIGHTS");
	ImGui::SliderFloat("Flux", &flux_, 0.0f, 100.0f);
	shape_->GUI();
}

float Light::getRadiosity(int)
{
	return 0.0f;
}
