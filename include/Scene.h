#pragma once

#include <vector>
#include <memory>
#include <lights/Light.h>
#include <shapes/Shape.h>
#include <materials/Material.h>
#include <Actor.h>
#include <cameras/Camera.h>
#include <Object.h>

class Scene {
public:
	std::vector<Object*> primitives;
	std::vector<Light*> lights;
	std::vector<Camera*> cameras_;
	int activeCamera = 0;
};