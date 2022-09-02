#pragma once

#include <integrators/Integrator.h>

class MonteCarloIntegrator : public Integrator {
public:
	MonteCarloIntegrator(Scene* scene);

	void Integrate(Texture& renderTexture);
	void GUI();
	glm::vec3 traceRay(Ray& r);
public:
	int maxRecursiveDepth_;
	int samplerPerPixel_;
	float bias_;
};