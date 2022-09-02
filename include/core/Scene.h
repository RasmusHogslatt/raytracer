#pragma once

#include <memory>
#include <vector>
#include <core/Primitive.h>
#include <lights/Light.h>
#include <cameras/Camera.h>
#include <samplers/Sampler.h>
#include <shapes/Sphere.h>

/// <summary>
/// Primitives:
/// 0 = sphere
/// 1 = triangle
/// 
/// Lights:
/// 0 = Sphere light
/// 1 = Area light
/// 2 = Point light
/// </summary>
class Scene {
public:
	Scene();

	void GUI();

public:
	std::vector<Primitive*> primitives_;
	std::vector<Light*> lights_;
	std::vector<Camera*> cameras_;
	std::vector<Sampler*> samplers_;

	// One of each primitive for GUI: 0 = Sphere, 1 = Triangle
	std::vector<Primitive*> dummyPrimitives_;

	int activeCamera_;
	int activeSampler_;
	int activePrimitive_;
	int activeLight_;

	Sphere dummySphere_;
	Light dummyLight_; // change to area light
};