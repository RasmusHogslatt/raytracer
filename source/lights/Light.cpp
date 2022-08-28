#pragma once

#include <lights/Light.h>

Light::Light() : Actor(), lightProperties_{}
{
}

Light::Light(const Light& old)
{
	position_ = old.position_;
	lightProperties_ = old.lightProperties_;
}

void Light::GUISettings()
{
	ImGui::Text("Current object type : Point Light");
	ImGui::SliderFloat3("Light position", &position_.x, -10, 10);
	ImGui::SliderFloat("Light intensity", &lightProperties_.intensity, 0, 10);
	ImGui::ColorPicker3("Light color", &lightProperties_.color.x);
}
