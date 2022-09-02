#pragma once

#include "Primitive.h"
#include <shapes/Sphere.h>

Primitive::Primitive() : material_{new Material()}, shape_{new Sphere()}
{
}

Primitive::Primitive(const Shape& oldShape, const Material& oldMaterial)
{
	shape_ = oldShape.clone();
	material_ = oldMaterial.clone();
}