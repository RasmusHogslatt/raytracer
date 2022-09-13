#pragma once

#include <core/Primitive.h>
#include <shapes/Sphere.h>
#include <imgui.h>

Primitive::Primitive(Material* material, Shape* shape) : material_{ material }, shape_{ shape } {}

Primitive::Primitive(const Primitive& old) {
	material_ = old.material_->clone();
	shape_ = old.shape_->clone();
}

Primitive* Primitive::clone() const
{
	return new Primitive(*this);
}

Primitive::~Primitive() {}

void Primitive::GUI()
{
	ImGui::NewLine();
	ImGui::Text("PRIMITIVES");
	shape_->GUI();
	material_->GUI();
}