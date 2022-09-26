#pragma once

#include <integrators/Integrator.h>
#include <glm/gtx/rotate_vector.hpp>

class MonteCarloIntegrator : public Integrator {
public:
	MonteCarloIntegrator(Scene* scene);

	void Integrate(Texture& renderTexture);
	void GUI();
	glm::vec3 traceRay(Ray& r);

	// Returns nearest intersected object index or -1 if no intersection found. 0 = primitive, 1 = light
	int getNearestIntersectionIndex(Ray& r, float& depth, int type = 0);

	// Returns direction inside hemisphere. Takes two random floats [0, 1]
	glm::vec3 uniformSampleHemisphere(const Ray& ray, const float& r1, const float& r2);

	glm::vec3 cosineWeightedSampleHemisphere(const Ray& ray, const float& r1, const float& r2);

	glm::vec3 directLight(Ray& ray);
	glm::vec3 indirectLight(Ray& ray);
	glm::vec3 traverseList(std::shared_ptr<Ray> r);
	bool russianRoulette(float probability);
public:
	int samplerPerPixel_;
	float bias_;
	int lightSamples_;
	int progress_;
	int indirectSampleOption_;
};