#pragma once

#include <vector>
#include <memory>
#include <lights/Light.h>
#include <shapes/Shape.h>
#include <materials/Material.h>
#include <Primitive.h>
#include <MyCamera.h>
#include <Actor.h>
class Scene {
public:
	std::vector<std::shared_ptr<Actor>> aggregate_;
	std::shared_ptr<MyCamera> cameras_;
};