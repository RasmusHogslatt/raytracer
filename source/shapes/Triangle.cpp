#pragma once

#include <shapes/Triangle.h>
#include <cmath>
#include <imgui.h>

Triangle::Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
{
	vertices_.push_back(v0);
	vertices_.push_back(v1);
	vertices_.push_back(v2);
	normal_ = getNormal(glm::vec3(0));
	area_ = getArea();
}

Triangle::Triangle(const Triangle& old)
{
	vertices_ = old.vertices_;
	area_ = area_;
	normal_ = old.normal_;
}

Triangle::~Triangle()
{

}

Triangle* Triangle::clone() const
{
	return new Triangle(*this);
}

bool Triangle::intersect(Ray& ray, float& t)
{
	// Edges
	glm::vec3 edge1 = vertices_[1] - vertices_[0];
	glm::vec3 edge2 = vertices_[2] - vertices_[0];
	glm::vec3 h = glm::cross(ray.direction_, edge2);
	float a = glm::dot(edge1, h);

	// Back face culling
	if (glm::dot(normal_, ray.direction_) >= 0) {
		return false;
	}

	// Ray is parallell to triangle
	if (a > FLT_EPSILON && a < FLT_EPSILON) {
		return false;
	}

	float f = 1.0f / a;
	glm::vec3 s = ray.origin_ - vertices_[0];
	float u = f * glm::dot(s, h);
	
	// Outside triangle
	if (u < 0.0f || u > 1.0f) {
		return false;
	}

	glm::vec3 q = glm::cross(s, edge1);
	float v = f * glm::dot(ray.direction_, q);
	
	// Outside triangle
	if (v < 0.0f || u + v > 1.0f) {
		return false;
	}

	// Compute t
	t = f * glm::dot(edge2, q);

	// Intersection in front of ray's origin
	if (t > FLT_EPSILON) {
		ray.setEnd(t);
		ray.intersectionNormal_ = getNormal(ray.end_);
		return true;
	}
	// Intersection behind ray's origin
	else {
		return false;
	}
}

glm::vec3 Triangle::getPosition()
{
	return (vertices_[0] + vertices_[1] + vertices_[2]) / 3.0f;
}

glm::vec3 Triangle::getNormal(const glm::vec3&)
{
	glm::vec3 edge1 = glm::normalize(vertices_[0] - vertices_[1]);
	glm::vec3 edge2 = glm::normalize(vertices_[2] - vertices_[1]);
	normal_ = glm::normalize(glm::cross(edge1, edge2));
	return normal_;
}

glm::vec3 Triangle::getPointOnSurface(const glm::vec3&, float u, float v)
{
	if (u + v > 1.0f) {
		u = 1.0f - u;
		v = 1.0f - v;
	}
	glm::vec3 edge1 = glm::normalize(vertices_[0] - vertices_[1]);
	glm::vec3 edge2 = glm::normalize(vertices_[2] - vertices_[1]);

	return vertices_[1] + edge1 * u + edge2 * v;
}

float Triangle::getArea()
{
	float edge1 = glm::length(vertices_[1] - vertices_[0]);
	float edge2 = glm::length(vertices_[2] - vertices_[0]);
	float edge3 = glm::length(vertices_[1] - vertices_[2]);
	float s = (edge1 + edge2 + edge3) / 2.0f;
	return std::sqrt(s * (s - edge1) * (s - edge2) * (s - edge3));
}

void Triangle::GUI()
{
	ImGui::SliderFloat3("Vertex 1", &vertices_[0].x, -10.0f, 10.0f);
	ImGui::SliderFloat3("Vertex 2", &vertices_[1].x, -10.0f, 10.0f);
	ImGui::SliderFloat3("Vertex 3", &vertices_[2].x, -10.0f, 10.0f);
	area_ = getArea();
	ImGui::NewLine();
}
