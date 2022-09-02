#pragma once

#include <core/Scene.h>
#include <imgui.h>
#include <iostream>
#include <samplers/CenterSampler.h>
#include <cameras/PerspectiveCamera.h>

Scene::Scene() : activeCamera_{ 0 }, activeSampler_{ 0 }, activePrimitive_{ 0 }, activeLight_{ 0 }, dummySphere_{ Sphere() }, dummyLight_{Light()} {
	// Add primitives

	// Add lights

	// Add cameras
	cameras_.push_back(new PerspectiveCamera());
	
	// Add samplers
	samplers_.push_back(new CenterSampler());
}

void Scene::GUI()
{
	ImGui::SliderInt("Primitive type", &activePrimitive_, 0, 0);
	ImGui::SliderInt("Light type", &activeLight_, 0, 0);
	ImGui::SliderInt("Camera type", &activeCamera_, 0, 0);
	ImGui::SliderInt("Sampler type", &activeSampler_, 0, 0);
	
	cameras_[activeCamera_]->GUI();
	samplers_[activeSampler_]->GUI();

	if (activePrimitive_ == 0) {
		dummySphere_.GUI();
	}
	else if (activePrimitive_ == 1) {
		// add triangle
	}
	if (activeLight_ == 0) {
		lights_.push_back(new Light(dummyLight_));
	}
	else if (activeLight_ == 1) {
		// add area light
	}
	else if (activeLight_ == 2) {
		// add point light
	}

	if (ImGui::Button("Add primitive")) {

		if (activePrimitive_ == 0) {
			primitives_.push_back(new Sphere(dummySphere_));
		}
		std::cout << primitives_.size() << "\n";
	}
}
