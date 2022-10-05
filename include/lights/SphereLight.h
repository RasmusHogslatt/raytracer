#pragma once

#include <lights/Light.h>
#include <shapes/Sphere.h>

class SphereLight : public Light {
public:
	SphereLight();
	SphereLight(const SphereLight& old);
	~SphereLight();
	void GUI();
	float getRadiosity(int samples);
public:
};