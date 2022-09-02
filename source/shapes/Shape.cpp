#pragma once

#include <shapes/Shape.h>
#include <imgui.h>

Shape::Shape() : position_{ glm::vec3(0) } {}

Shape::Shape(const Shape& old)
{
	position_ = old.position_;
}

void Shape::GUI()
{
	ImGui::SliderFloat3("Position", &position_.x, -10.0f, 10.0f);
}
