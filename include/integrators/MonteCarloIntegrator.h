#pragma once

#include <integrators/Integrator.h>
#include <glm/gtx/rotate_vector.hpp>

class MonteCarloIntegrator : public Integrator {
public:
	MonteCarloIntegrator(Scene* scene);

	void Integrate(Texture& renderTexture);
	void GUI();
	glm::vec3 traceRay(Ray& r, int depth);

	// Returns nearest intersected object index or -1 if no intersection found. 0 = primitive, 1 = light
	int getNearestIntersectionIndex(Ray& r, float& depth, int type = 0);

	// Returns direction inside hemisphere. Takes two random floats [0, 1]
	glm::vec3 uniformSampleHemisphere(const glm::vec3& normal, const float& r1, const float& r2);

	glm::vec3 directLight(const Ray& ray);
	glm::vec3 indirectLight(const Ray& ray, int& depth);
	glm::vec3 traverseTree(const Ray& root);
public:
	int maxRecursiveDepth_;
	int samplerPerPixel_;
	float bias_;
	int indirectLightSamplesOnHemisphere_;
	int lightSamples_;
	int progress_;
};