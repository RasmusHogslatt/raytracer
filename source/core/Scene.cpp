#pragma once

#include <core/Scene.h>
#include <imgui.h>
#include <iostream>
#include <samplers/CenterSampler.h>
#include <cameras/PerspectiveCamera.h>
#include <lights/SphereLight.h>
#include <shapes/Triangle.h>
#include <shapes/Rectangle.h>

Scene::Scene() : activeCamera_{ 0 }, activeSampler_{ 0 }, activePrimitive_{ 0 }, activeLight_{ 0 }, dummySphere_{ Primitive() }, dummyTriangle_{ Primitive() }, dummyRectangle_{ Primitive() }, dummySphereLight_{ SphereLight() } {
	dummySphere_.shape_ = new Sphere();
	dummyTriangle_.shape_ = new Triangle();
	dummyRectangle_.shape_ = new Rectangle();
	dummySphereLight_.shape_ = new Sphere();
	// Add primitives

	// Add lights


	// Add cameras
	cameras_.push_back(new PerspectiveCamera());

	// Add samplers
	samplers_.push_back(new CenterSampler());

	loadRoom();
}

void Scene::GUI()
{
	ImGui::SliderInt("Primitive type", &activePrimitive_, 0, 2);
	ImGui::SliderInt("Light type", &activeLight_, 0, 0);
	ImGui::SliderInt("Camera type", &activeCamera_, 0, 0);
	ImGui::SliderInt("Sampler type", &activeSampler_, 0, 0);

	cameras_[activeCamera_]->GUI();
	samplers_[activeSampler_]->GUI();

	if (activePrimitive_ == 0) {
		dummySphere_.GUI();
	}
	else if (activePrimitive_ == 1) {
		dummyTriangle_.GUI();
	}
	else if (activePrimitive_ == 2) {
		dummyRectangle_.GUI();
	}
	if (activeLight_ == 0) {
		dummySphereLight_.GUI();
	}
	else if (activeLight_ == 1) {
		// add area light
	}
	else if (activeLight_ == 2) {
		// add point light
	}

	if (ImGui::Button("Add primitive")) {

		if (activePrimitive_ == 0) {
			primitives_.push_back(dummySphere_.clone());
		}
		else if (activePrimitive_ == 1) {
			primitives_.push_back(dummyTriangle_.clone());
		}
		else if (activePrimitive_ == 2) {
			primitives_.push_back(dummyRectangle_.clone());
		}
	}
	if (ImGui::Button("Add light")) {
		if (activeLight_ == 0) {
			lights_.push_back(dummySphereLight_.clone());
		}
	}
}

void Scene::loadRoom()
{
	// 3 balls
	Sphere diffuse = Sphere(glm::vec3(0, 0, 0), 0.5f);
	Sphere reflective = Sphere(glm::vec3(1.1, 0, 0), 0.5f);
	Sphere transmissive = Sphere(glm::vec3(-1.1, 0, 0), 0.5f);
	Material b1 = Material(); b1.color_ = glm::vec3(0, 0.5f, 1.0f);
	Material b2 = Material(); b2.reflectance_ = 1.0f; b2.transmittance_ = 0.0f; b2.color_ = glm::vec3(1);
	Material b3 = Material(); b3.transmittance_ = 1.0f;
	Primitive pb1 = Primitive(&b1, &diffuse);
	Primitive pb2 = Primitive(&b2, &reflective);
	Primitive pb3 = Primitive(&b3, &transmissive);
	primitives_.push_back(pb1.clone());
	primitives_.push_back(pb2.clone());
	primitives_.push_back(pb3.clone());

	// Shapes
	Rectangle roof = Rectangle(glm::vec3(6, 5, 8), glm::vec3(6, 5, -8), glm::vec3(-6, 5, -8));
	Rectangle floor = Rectangle(glm::vec3(-6, -5, -8), glm::vec3(6, -5, -8), glm::vec3(6, -5, 8));
	Rectangle backWall1 = Rectangle(glm::vec3(6, -5, 5), glm::vec3(6, 5, 5), glm::vec3(0, 5, 8));
	Rectangle backWall2 = Rectangle(glm::vec3(0, -5, 8), glm::vec3(0, 5, 8), glm::vec3(-6, 5, 5));
	Rectangle sideWall1 = Rectangle(glm::vec3(-6, -5, 5), glm::vec3(-6, 5, 5), glm::vec3(-6, 5, -5));
	Rectangle sideWall2 = Rectangle(glm::vec3(6, -5, -5), glm::vec3(6, 5, -5), glm::vec3(6, 5, 5));
	Rectangle frontWall1 = Rectangle(glm::vec3(-6, -5, -5), glm::vec3(-6, 5, -5), glm::vec3(0, 5, -8));
	Rectangle frontWall2 = Rectangle(glm::vec3(0, -5, -8), glm::vec3(0, 5, -8), glm::vec3(6, 5, -5));

	// Materials
	Material m1 = Material();
	Material m2 = Material();
	Material m3 = Material();
	Material m4 = Material();
	Material m5 = Material();
	Material m6 = Material();
	Material m7 = Material();
	Material m8 = Material();

	//Primitives
	Primitive p1;
	p1.shape_ = &roof;
	m1.color_ = glm::vec3(0.3, 0.3, 0.3);
	p1.material_ = &m1;

	Primitive p2;
	p2.shape_ = &floor;
	p2.material_ = &m1;

	Primitive p3;
	p3.shape_ = &backWall1;
	m3.color_ = glm::vec3(0.5, 0.0, 0);
	p3.material_ = &m3;

	Primitive p4;
	p4.shape_ = &backWall2;
	m4.color_ = glm::vec3(0.5, 0, 0.0);
	p4.material_ = &m4;

	Primitive p5;
	p5.shape_ = &sideWall1;
	m5.color_ = glm::vec3(0., 0.5, 0.0);
	p5.material_ = &m5;

	Primitive p6;
	p6.shape_ = &sideWall2;
	m6.color_ = glm::vec3(0, 0, 0.5);
	p6.material_ = &m6;

	Primitive p7;
	p7.shape_ = &frontWall1;
	m7.color_ = glm::vec3(0.2, 0, 0.3);
	p7.material_ = &m7;

	Primitive p8;
	p8.shape_ = &frontWall2;
	m8.color_ = glm::vec3(0.2, 0.3, 0);
	p8.material_ = &m8;

	primitives_.push_back(p1.clone());
	primitives_.push_back(p2.clone());
	primitives_.push_back(p3.clone());
	primitives_.push_back(p4.clone());
	primitives_.push_back(p5.clone());
	primitives_.push_back(p6.clone());
	primitives_.push_back(p7.clone());
	primitives_.push_back(p8.clone());

	// Lights
	Light sl1 = Light();
	Light sl2 = Light();
	Light sl3 = Light();
	Sphere s1 = Sphere(glm::vec3(0.0f, 3.5, 0.0f), 1.0f);
	Sphere s2 = Sphere(glm::vec3(2.0f, 3.0f, 4.0f), 1.0f);
	Sphere s3 = Sphere(glm::vec3(-2.0f, 1.5, 3.0f), 1.0f);
	sl1.shape_ = &s1;
	sl1.flux_ = 100;
	sl2.shape_ = &s2;
	sl2.flux_ = 100;
	sl3.shape_ = &s3;
	sl3.flux_ = 100;
	lights_.push_back(sl1.clone());
	lights_.push_back(sl2.clone());
	lights_.push_back(sl3.clone());
}
