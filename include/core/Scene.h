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

	void loadRoom();
public:
	std::vector<Primitive*> primitives_;
	std::vector<Light*> lights_;
	std::vector<Camera*> cameras_;
	std::vector<Sampler*> samplers_;



	int activeCamera_;
	int activeSampler_;
	int activePrimitive_;
	int activeLight_;

	Primitive dummySphere_;
	Primitive dummyTriangle_;
	Primitive dummyRectangle_;
	Light dummySphereLight_;
};