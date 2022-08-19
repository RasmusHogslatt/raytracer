#pragma once

#include <vector>
#include <memory>
#include <lights/Light.h>
#include <shapes/Shape.h>
#include <materials/Material.h>
#include <Actor.h>
#include <cameras/Camera.h>

class Scene {
public:
	std::vector<Actor*> primitives;
	std::vector<Camera*> cameras_;
};