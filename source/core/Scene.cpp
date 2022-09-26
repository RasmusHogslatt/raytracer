#pragma once

#include <core/Scene.h>
#include <imgui.h>
#include <iostream>
#include <samplers/CenterSampler.h>
#include <samplers/RandomSampler.h>
#include <cameras/PerspectiveCamera.h>
#include <lights/SphereLight.h>
#include <shapes/Triangle.h>
#include <shapes/Rectangle.h>
#include <shapes/Tetrahedron.h>

Scene::Scene() : activeCamera_{ 0 }, activeSampler_{ 1 }, activePrimitive_{ 0 }, activeLight_{ 0 }, dummySphere_{ Primitive() }, dummyTriangle_{ Primitive() }, dummyRectangle_{ Primitive() }, dummyTetrahedron_{ Primitive() }, dummySphereLight_{ SphereLight() }, activeMaterial_{ 0 } {
	materials_.push_back(new Lambertian());
	materials_.push_back(new Mirror());
	materials_.push_back(new Glass());
	dummySphere_.shape_ = new Sphere(); dummySphere_.material_ = materials_[activeMaterial_];
	dummyTriangle_.shape_ = new Triangle(); dummyTriangle_.material_ = materials_[activeMaterial_];
	dummyRectangle_.shape_ = new Rectangle(); dummyRectangle_.material_ = materials_[activeMaterial_];
	dummyTetrahedron_.shape_ = new Tetrahedron(); dummyTetrahedron_.material_ = materials_[activeMaterial_];
	dummySphereLight_.shape_ = new Sphere();
	// Add primitives

	// Add lights

	// Add cameras
	cameras_.push_back(new PerspectiveCamera());

	// Add samplers
	samplers_.push_back(new CenterSampler());
	samplers_.push_back(new RandomSampler());

	loadRoom();
}

void Scene::GUI()
{
	ImGui::NewLine();
	ImGui::Text("SCENE");
	ImGui::NewLine();

	ImGui::Text("SAMPLER");
	ImGui::SliderInt("Sampler type : ", &activeSampler_, 0, 1);
	switch (activeSampler_) {
	case 0:
		ImGui::SameLine(); ImGui::Text("Center");
		break;
	case 1:
		ImGui::SameLine(); ImGui::Text("Random");
		break;
	}
	samplers_[activeSampler_]->GUI();

	ImGui::NewLine();
	ImGui::Text("CAMERA");
	ImGui::SliderInt("Camera type", &activeCamera_, 0, 0);
	cameras_[activeCamera_]->GUI();

	ImGui::NewLine();
	ImGui::Text("OBJECTS");
	ImGui::SliderInt("Shape : ", &activePrimitive_, 0, 3);
	switch (activePrimitive_) {
	case 0:
		ImGui::SameLine(); ImGui::Text("Sphere");
		dummySphere_.GUI();
		break;
	case 1:
		ImGui::SameLine(); ImGui::Text("Triangle");
		dummyTriangle_.GUI();
		break;
	case 2:
		ImGui::SameLine(); ImGui::Text("Rectangle");
		dummyRectangle_.GUI();
		break;
	case 4:
		ImGui::SameLine(); ImGui::Text("Tetrahedron");
		dummyTetrahedron_.GUI();
		break;
	}

	ImGui::NewLine();
	ImGui::Text("MATERIAL");
	ImGui::SliderInt("Material : ", &activeMaterial_, 0, 2);
	ImGui::SameLine();
	materials_[activeMaterial_]->GUI();

	ImGui::NewLine();
	ImGui::Text("LIGHTS");
	ImGui::SliderInt("Light type : ", &activeLight_, 0, 0);
	switch (activeLight_) {
	case 0:
		ImGui::SameLine(); ImGui::Text("Sphere light");
		dummySphereLight_.GUI();
		break;
	}

	ImGui::NewLine();
	if (ImGui::Button("ADD OBJECT")) {

		if (activePrimitive_ == 0) {
			dummySphere_.material_ = materials_[activeMaterial_]->clone();
			primitives_.push_back(dummySphere_.clone());
		}
		else if (activePrimitive_ == 1) {
			dummyTriangle_.material_ = materials_[activeMaterial_]->clone();
			primitives_.push_back(dummyTriangle_.clone());
		}
		else if (activePrimitive_ == 2) {
			dummyRectangle_.material_ = materials_[activeMaterial_]->clone();
			primitives_.push_back(dummyRectangle_.clone());
		}
		else if (activePrimitive_ == 3) {
			dummyTetrahedron_.material_ = materials_[activeMaterial_]->clone();
			primitives_.push_back(dummyTetrahedron_.clone());
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("ADD LIGHT")) {
		if (activeLight_ == 0) {
			lights_.push_back(dummySphereLight_.clone());
		}
	}
}

void Scene::loadRoom()
{
	//// 3 balls
	//Sphere diffuse = Sphere(glm::vec3(0, 0, 0), 0.5f);
	//Sphere reflective = Sphere(glm::vec3(1.1, 0, 0), 0.5f);
	//Sphere transmissive = Sphere(glm::vec3(-1.1, 0, 0), 0.5f);
	//Lambertian b1 = Lambertian();
	//Mirror b2 = Mirror();
	//Glass b3 = Glass();
	//Primitive pb1 = Primitive(&b1, &diffuse);
	//Primitive pb2 = Primitive(&b2, &reflective);
	//Primitive pb3 = Primitive(&b3, &transmissive);
	//primitives_.push_back(pb1.clone());
	//primitives_.push_back(pb2.clone());
	//primitives_.push_back(pb3.clone());

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
	Lambertian m1 = Lambertian();
	Lambertian m2 = Lambertian();
	Lambertian m3 = Lambertian();
	Lambertian m4 = Lambertian();
	Lambertian m5 = Lambertian();
	Lambertian m6 = Lambertian();
	Lambertian m7 = Lambertian();
	Lambertian m8 = Lambertian();

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
	Sphere s2 = Sphere(glm::vec3(3.0f, 3.0f, 4.0f), 1.0f);
	Sphere s3 = Sphere(glm::vec3(-3.0f, 0.5, 3.0f), 1.0f);
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
