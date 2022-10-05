#pragma once

#include <lights/Light.h>
#include <shapes/Rectangle.h>

class RectangleLight : public Light {
public:
	RectangleLight();
	RectangleLight(const RectangleLight& old);
	~RectangleLight();
	void GUI();
	float getRadiosity(int samples);
public:
};