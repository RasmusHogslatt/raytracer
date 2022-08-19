#pragma once
#include "MySphere.h"

MySphere::MySphere(float radius) : radius_ { radius } {
}

MySphere::MySphere(const MySphere& old)
{
	position_ = old.position_;
	material_ = old.material_;
	normal_ = old.normal_;
	radius_ = old.radius_;
}

bool MySphere::intersect(const Ray& ray)
{
	glm::vec3 l = position_ - ray.origin;
	float tca = glm::dot(l, ray.direction);
	float d2 = glm::dot(l, l) - tca * tca;
	if (d2 > radius_ * radius_) {
		return false;
	}
	//float thc = sqrt(radius_* radius_ - d2);
	//float t0 = tca - thc;
	//float t1 = tca + thc;
	//
	//if (t0 > t1) std::swap(t0, t1);

	//if (t0 < 0) {
	//	t0 = t1;  //if t0 is negative, let's use t1 instead 
	//	if (t0 < 0) return false;  //both t0 and t1 are negative 
	//}

	//float t = t0;

	return true;
}

void MySphere::GUISettings()
{
	ImGui::Text("Current object type : Sphere");
	ImGui::SliderFloat3("Actor position", &position_.x, -10, 10);
	ImGui::SliderFloat("Radius", &radius_, 0, 50);
	material_.GUISettings();
}
