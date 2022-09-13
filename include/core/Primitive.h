#pragma once

#include <materials/Material.h>
#include <core/Ray.h>
#include <shapes/Shape.h>

class Primitive {
public:
	Primitive(Material* material = new Material(), Shape* shape = nullptr);
	Primitive(const Primitive& old);
	Primitive* clone() const;
	virtual ~Primitive();

	void GUI();
public:
	Material* material_;
	Shape* shape_;
};