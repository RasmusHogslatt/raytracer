#pragma once

#include <Object.h>

class MySphere : public Object {
public:
	MySphere(float radius = 1.0);
	MySphere(const MySphere& old);
	bool intersect(const Ray& ray);
	void GUISettings();
public:
	float radius_;
};