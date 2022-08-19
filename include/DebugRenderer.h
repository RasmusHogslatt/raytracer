#pragma once

#include <integrators/Integrator.h>
#include <Actor.h>
#include <vector>
#include <Texture.h>
#include <Parameters.h>
#include <Parameters.h>

class DebugRenderer : public Integrator {
public:
	DebugRenderer(Parameters& params);
	
	void Render(const Scene& scene);
	void GUISettings();
private:
	Parameters& p;
	int samples = 1;
};