#pragma once

#include <shapes/Sphere.h>
#include <imgui.h>
#include <glm/gtx/rotate_vector.hpp>

Sphere::Sphere(glm::vec3 position, float radius) : Shape(), position_{ position }, radius_{ radius }, area_{ getArea() } {}

Sphere::Sphere(const Sphere& old) {
	position_ = old.position_;
	radius_ = old.radius_;
	area_ = old.area_;
}

Sphere* Sphere::clone() const
{
	return new Sphere(*this);
}

Sphere::~Sphere()
{
}

bool Sphere::intersect(Ray& ray, float& t)
{
	float pHalf = glm::dot((ray.origin_ - position_), ray.direction_);
	float toSquare = pow(pHalf, 2.0f) + pow(radius_, 2.0f) - pow(glm::length(ray.origin_ - position_), 2.0f);
	
	glm::vec3 normal;

	// No intersection
	if (toSquare < 0) {
		return false;
	}
	// One or two intersections
	else {
		t = -pHalf - sqrt(toSquare);
		//Inside sphere
		if (t < 0) {
			t = -pHalf + sqrt(toSquare);
		}
		normal = ray.origin_ + t * ray.direction_ - position_;
	}

	// t positive required to travel forward in scene
	if (t >= 0) {
		return true;
	}
	return false;
}

glm::vec3 Sphere::getPosition()
{
	return position_;
}

float Sphere::getArea()
{
	return radius_ * radius_ * 3.14f * 4.0f;
}

glm::vec3 Sphere::getNormal(const glm::vec3& intersectionPoint)
{
	return glm::normalize(intersectionPoint - position_);
}

glm::vec3 Sphere::getPointOnHemisphere(float u, float v) const
{
	float inclination = glm::acos(1 - 2 * u);
	float azimuth = 2.0f * 3.14f * v;
	glm::vec3 randomDirection = glm::vec3(1, 0, 0);

	randomDirection = glm::normalize(glm::rotate(
		randomDirection,
		inclination,
		glm::vec3(0, 1, 0)));
	randomDirection = glm::normalize(glm::rotate(
		randomDirection,
		azimuth,
		glm::vec3(1, 0, 0)));
	return position_ + randomDirection * radius_;
}

void Sphere::GUI()
{
	ImGui::Text("Shape: SPHERE");
	ImGui::PushID(ImGui::SliderFloat3("Sphere position", &position_.x, -10.0f, 10.0f));
	if (ImGui::SliderFloat("Radius", &radius_, 0.1f, 10.0f)) {
		area_ = getArea();
	}
	ImGui::NewLine();
}
