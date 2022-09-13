#pragma once

#include <shapes/Shape.h>

class Light {
public:
	Light(Shape* shape = nullptr);
	Light(const Light& old);
	Light* clone() const;
	virtual ~Light();
	void GUI();
	float getRadiosity();
public:
	Shape* shape_;
	float flux_;
};