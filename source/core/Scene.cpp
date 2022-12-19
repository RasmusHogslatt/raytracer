#pragma once

#include <core/Scene.h>
#include <imgui.h>
#include <iostream>
#include <samplers/CenterSampler.h>
#include <samplers/RandomSampler.h>
#include <cameras/PerspectiveCamera.h>
#include <lights/SphereLight.h>
#include <lights/RectangleLight.h>
#include <shapes/Triangle.h>
#include <shapes/Rectangle.h>
#include <shapes/Tetrahedron.h>
#include <ResourcePath.h>

Scene::Scene() : activeCamera_{ 0 }, activeSampler_{ 1 }, activePrimitive_{ 0 }, activeLight_{ 0 }, dummySphere_{ Primitive() }, dummyTriangle_{ Primitive() }, dummyRectangle_{ Primitive() }, dummyTetrahedron_{ Primitive() }, dummySphereLight_{ SphereLight() }, dummyRectangleLight_{ RectangleLight() }, activeMaterial_{ 0 } {
	materials_.push_back(new Lambertian());
	materials_.push_back(new Mirror());
	materials_.push_back(new Glass());
	dummySphere_.shape_ = new Sphere(); dummySphere_.material_ = materials_[activeMaterial_];
	dummyTriangle_.shape_ = new Triangle(); dummyTriangle_.material_ = materials_[activeMaterial_];
	dummyRectangle_.shape_ = new Rectangle(); dummyRectangle_.material_ = materials_[activeMaterial_];
	dummyTetrahedron_.shape_ = new Tetrahedron(); dummyTetrahedron_.material_ = materials_[activeMaterial_];
	dummySphereLight_.shape_ = new Sphere();
	dummyRectangleLight_.shape_ = new Rectangle();
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
	ImGui::SliderInt("Light type : ", &activeLight_, 0, 1);
	switch (activeLight_) {
	case 0:
		ImGui::SameLine(); ImGui::Text("Sphere light");
		dummySphereLight_.GUI();
		break;
	case 1:
		ImGui::SameLine(); ImGui::Text("Rectangle light");
		dummyRectangleLight_.GUI();
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
		else if (activeLight_ == 1) {
			lights_.push_back(dummyRectangleLight_.clone());
		}
	}
}

void Scene::loadRoom()
{
	//// 3 balls
	Sphere diffuse = Sphere(glm::vec3(0.5, 0, 0), 0.5f);
	Sphere diffuse2 = Sphere(glm::vec3(-3.0f, -2.0f, 1.0f), 0.75f);
	Sphere diffuse3 = Sphere(glm::vec3(-1.0f, -1.0f, 3.0f), 0.3f);
	Sphere diffuse4 = Sphere(glm::vec3(-2.3f, -0.3f, 4.0f), 0.45f);
	Sphere diffuse5 = Sphere(glm::vec3(-2.0f, 1.2f, 1.0f), 0.2f);
	Sphere reflective = Sphere(glm::vec3(3.0f, 0, 0), 1.2f);
	Sphere transmissive = Sphere(glm::vec3(-1.1, 0, 0), 0.7f);
	Lambertian b1 = Lambertian();
	Lambertian b12 = Lambertian(); b12.color_ = glm::vec3(0, 0, 1);
	Lambertian b13 = Lambertian(); b13.color_ = glm::vec3(0, 1, 0);
	Mirror b14 = Mirror();
	Lambertian b15 = Lambertian(); b15.color_ = glm::vec3(0.4, 0.5, 0.1);
	Mirror b2 = Mirror();
	Glass b3 = Glass();
	Primitive pb1 = Primitive(&b1, &diffuse);
	Primitive pb12 = Primitive(&b12, &diffuse2);
	Primitive pb13 = Primitive(&b13, &diffuse3);
	Primitive pb14 = Primitive(&b14, &diffuse4);
	Primitive pb15 = Primitive(&b15, &diffuse5);
	Primitive pb2 = Primitive(&b2, &reflective);
	Primitive pb3 = Primitive(&b3, &transmissive);
	primitives_.push_back(pb1.clone());
	primitives_.push_back(pb12.clone());
	primitives_.push_back(pb13.clone());
	primitives_.push_back(pb14.clone());
	primitives_.push_back(pb15.clone());
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
	m1.color_ = glm::vec3(0.1, 0.1, 0.1);
	p1.material_ = &m1;

	Primitive p2;
	p2.shape_ = &floor;
	p2.material_ = &m1;

	Primitive p3;
	p3.shape_ = &backWall1;
	m3.color_ = glm::vec3(0.3, 0.6, 0.2);
	p3.material_ = &m3;

	Primitive p4;
	p4.shape_ = &backWall2;
	m4.color_ = glm::vec3(0.6, 0.3, 0.0);
	p4.material_ = &m4;

	Primitive p5;
	p5.shape_ = &sideWall1;
	m5.color_ = glm::vec3(0.1, 0.3, 0.2);
	p5.material_ = &m5;

	Primitive p6;
	p6.shape_ = &sideWall2;
	m6.color_ = glm::vec3(0.2, 0.1, 0.5);
	p6.material_ = &m6;

	Primitive p7;
	p7.shape_ = &frontWall1;
	m7.color_ = glm::vec3(0.3, 0.3, 0.0);
	p7.material_ = &m7;

	Primitive p8;
	p8.shape_ = &frontWall2;
	m8.color_ = glm::vec3(0.1, 0.45, 0.4);
	p8.material_ = &m8;

	primitives_.push_back(p1.clone());
	primitives_.push_back(p2.clone());
	primitives_.push_back(p3.clone());
	primitives_.push_back(p4.clone());
	primitives_.push_back(p5.clone());
	primitives_.push_back(p6.clone());
	primitives_.push_back(p7.clone());
	primitives_.push_back(p8.clone());

	Primitive tetra;
	glm::vec3 translate = glm::vec3(4, -5, -3);
	tetra.shape_ = new Tetrahedron(glm::vec3(sqrt(8.0f / 9.0f), -1.0f / 3.0f, 0.0f) + translate, glm::vec3(-sqrt(2.0f / 9.0f), -1.f / 3.f, sqrt(2.f / 3.f)) + translate, glm::vec3(-sqrt(2.f / 9.f), -1.f / 3.f, -sqrt(2.f / 3.f)) + translate, glm::vec3(0.0f, 1.0f, 0.0f) + translate);
	tetra.material_ = new Mirror();

	primitives_.push_back(tetra.clone());
	
	Primitive tetra2;
	glm::vec3 translate2 = glm::vec3(-4, -0, -5);
	tetra2.shape_ = new Tetrahedron(glm::vec3(sqrt(8.0f / 9.0f), -1.0f / 3.0f, 0.0f) + translate2, glm::vec3(-sqrt(2.0f / 9.0f), -1.f / 3.f, sqrt(2.f / 3.f)) + translate2, glm::vec3(-sqrt(2.f / 9.f), -1.f / 3.f, -sqrt(2.f / 3.f)) + translate2, glm::vec3(0.0f, 1.0f, 0.0f) + translate2);
	tetra2.material_ = new Lambertian();
	primitives_.push_back(tetra2.clone());

	// Lights
	Light sl1 = Light();
	Light sl2 = Light();
	Light sl3 = Light();
	Rectangle r1 = Rectangle(glm::vec3(-1.0f, 4.99f, -1), glm::vec3(-1.0f, 4.99f, 1.0f), glm::vec3(1.0f, 4.99f, 1.0f));
	Rectangle r2 = Rectangle(glm::vec3(-1.0f, 4.99f, -4.0f), glm::vec3(-1.0f, 4.99f, -2.0f), glm::vec3(1.0f, 4.99f, -2.0f));
	Rectangle r3 = Rectangle(glm::vec3(-1.0f, 4.99f, 2.0f), glm::vec3(-1.0f, 4.99f, 4.0f), glm::vec3(1.0f, 4.99f, 4.0f));
	Rectangle r4 = Rectangle(glm::vec3(-1.5f, -1.0f, 9.0f), glm::vec3(-1.5f, 1.0f, 9.0f), glm::vec3(1.5f, 41.0f, 9.0f));
	Sphere s1 = Sphere(glm::vec3(-3.0f, 2.0f, 1.0f), 0.1f);
	Sphere s2 = Sphere(glm::vec3(3.0f, 3.0f, 4.0f), 1.0f);
	Sphere s3 = Sphere(glm::vec3(-3.0f, 0.5, 3.0f), 1.0f);
	sl1.shape_ = &r1;
	sl1.flux_ = 25;
	sl2.shape_ = &r2;
	sl2.flux_ = 25;
	sl3.shape_ = &r3;
	sl3.flux_ = 25;
	lights_.push_back(sl1.clone());
	lights_.push_back(sl2.clone());
	lights_.push_back(sl3.clone());
}
