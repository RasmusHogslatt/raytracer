#pragma once

#include <Scene.h>

class Integrator {
public:
	virtual void Render(const Scene& scene) = 0;
	virtual void GUISettings() = 0;
};