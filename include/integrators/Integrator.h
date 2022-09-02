#pragma once

#include <core/Scene.h>
#include <core/Texture.h>

class Integrator {
public:
	Integrator(Scene* scene);

	virtual void Integrate(Texture& renderTexture) = 0;
	virtual void GUI() = 0;

public:
	Scene* scene_;
};