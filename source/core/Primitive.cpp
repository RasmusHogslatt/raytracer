#pragma once

#include <core/Primitive.h>
#include <shapes/Sphere.h>
#include <iostream>

Primitive::Primitive(Material* material) : material_{ material } {}

Primitive::Primitive(const Primitive& old) {
	material_ = old.material_;
}

Primitive::~Primitive() {}
