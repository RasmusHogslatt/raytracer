#pragma once

#include <shapes/Sphere.h>
#include <imgui.h>
#include <glm/gtx/rotate_vector.hpp>

Sphere::Sphere(glm::vec3 position, float radius) : Primitive(), position_{ position }, radius_{ radius } {}

Sphere::Sphere(const Sphere& old) {
	material_ = old.material_;
	position_ = old.position_;
	radius_ = old.radius_;
}

bool Sphere::intersect(Ray& ray)
{
	float pHalf = glm::dot((ray.origin_ - position_), ray.direction_);
	float toSquare = pow(pHalf, 2.0f) + pow(radius_, 2.0f) - pow(glm::length(ray.origin_ - position_), 2.0f);
	float t;
	glm::vec3 normal;

	// No intersection
	if (toSquare < 0) {
		return false;
	}
	// One or two intersections
	else {
		t = -pHalf + sqrt(toSquare);
		//Inside sphere
		if (t < 0) {
			t = -pHalf + sqrt(toSquare);
		}
		normal = ray.origin_ + t * ray.direction_ - position_;
	}

	// t positive required to travel forward in scene
	if (t >= 0) {
		// Update origin to intersection point
		ray.origin_ = ray.origin_ + ray.direction_ * t;
		return true;
	}
	return false;
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
	ImGui::SliderFloat3("Sphere position", &position_.x, -10.0f, 10.0f);
	ImGui::SliderFloat("Radius", &radius_, 0.1f, 10.0f);
	ImGui::NewLine();
	material_->GUI();
}
