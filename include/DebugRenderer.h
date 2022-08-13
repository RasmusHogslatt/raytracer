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
	glm::vec2 getSamplePosition(int x, int y, int currentSample);
private:
	Parameters& p;
};