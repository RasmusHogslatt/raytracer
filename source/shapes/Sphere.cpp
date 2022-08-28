#pragma once

#include <shapes/Sphere.h>

Sphere::Sphere(float radius) : radius_{ radius } {
}

Sphere::Sphere(const Sphere& old)
{
	position_ = old.position_;
	material_ = old.material_;
	normal_ = old.normal_;
	radius_ = old.radius_;
}

bool Sphere::intersect(const Ray& ray, float& distance)
{
	glm::vec3 l = position_ - ray.origin;
	float tca = glm::dot(l, ray.direction);
	float d2 = glm::dot(l, l) - tca * tca;
	if (d2 > radius_ * radius_) {
		t = -1;
		return false;
	}
	// If ray and normal are same direction, ray is inside, else, outside.
	// Check inside and outside for spheres specifically
	float thc = sqrt(radius_ * radius_ - d2);
	float t0 = tca - thc;
	float t1 = tca + thc;

	if (t0 > t1) {
		std::swap(t0, t1);
	}
	if (t0 < 0) {
		t0 = t1;  // if close is negative, use far instead
		if (t0 < 0) { // if close and far are negative, set to -1
			t = -1;
			return false;
		}
	}
	t = t0;
	distance = t0;

	return true;
}

void Sphere::GUISettings()
{
	ImGui::Text("Current object type : Sphere");
	ImGui::SliderFloat3("Actor position", &position_.x, -10, 10);
	ImGui::SliderFloat("Radius", &radius_, 0, 10);
	material_.GUISettings();
}
