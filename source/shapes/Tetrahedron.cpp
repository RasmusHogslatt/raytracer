#pragma once

#include <shapes/Tetrahedron.h>
#include <imgui.h>

Tetrahedron::Tetrahedron(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& v4) : Shape()
{
	triangles_.push_back(new Triangle(v4, v1, v2)); // FRONT
	triangles_.push_back(new Triangle(v4, v2, v3)); // LEFT
	triangles_.push_back(new Triangle(v4, v3, v1));
	triangles_.push_back(new Triangle(v1, v3, v2));
	area_ = 1.0f;
	intersectedIndex_ = -1;
}

Tetrahedron* Tetrahedron::clone() const
{
	return new Tetrahedron(*this);
}

bool Tetrahedron::intersect(Ray& ray, float& t)
{
	intersectedIndex_ = -1;
	float distance = std::numeric_limits<float>::max();
	for (int i = 0; i < 4; ++i) {
		float currentDepth = 0.0f;
		if (triangles_[i]->intersect(ray, currentDepth)) {
			if (currentDepth < distance) {
				distance = currentDepth;
				intersectedIndex_ = i;
				t = distance;
			}
		}
	}
	if (t > FLT_EPSILON && intersectedIndex_ != -1) {
		ray.setEnd(t);
		ray.intersectionNormal_ = triangles_[intersectedIndex_]->getNormal(glm::vec3(0));
		return true;
	}
	return false;
}

glm::vec3 Tetrahedron::getPosition()
{
	return glm::vec3();
}

glm::vec3 Tetrahedron::getNormal(const glm::vec3&)
{
	if (intersectedIndex_ != -1) {
		return triangles_[intersectedIndex_]->getNormal(glm::vec3(0));
	}
	return glm::vec3(1, 0, 0);
}

float Tetrahedron::getArea()
{
	return area_;
}

void Tetrahedron::GUI()
{
}

glm::vec3 Tetrahedron::getPointOnSurface(const glm::vec3&, float, float)
{
	return glm::vec3(0);
}
