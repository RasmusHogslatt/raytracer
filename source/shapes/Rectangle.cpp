#pragma once

#include <shapes/Rectangle.h>
#include <imgui.h>

Rectangle::Rectangle(const glm::vec3 bl, const glm::vec3 tl, const glm::vec3 tr)
{
	vertices_.push_back(bl);
	vertices_.push_back(tl);
	vertices_.push_back(tr);
	normal_ = getNormal(glm::vec3(0));
	area_ = getArea();
}

Rectangle::Rectangle(const Rectangle& old)
{
	vertices_ = old.vertices_;
	area_ = old.area_;
	normal_ = old.normal_;
}

Rectangle::~Rectangle()
{
	
}

Rectangle* Rectangle::clone() const
{
	return new Rectangle(*this);
}

bool Rectangle::intersect(Ray& ray, float& t)
{
	if (glm::dot(normal_, ray.direction_) >= 0) {
		return false;
	}

	// Edges share vertices_[1]
	glm::vec3 edge1 = vertices_[0] - vertices_[1];
	glm::vec3 edge2 = vertices_[2] - vertices_[1];

	glm::vec3 P = glm::cross(ray.direction_, edge2);

	float det = glm::dot(edge1, P);

	if (fabs(det) < FLT_EPSILON) {
		return false;
	}
	float invDet = 1.0f / det;

	glm::vec3 T = ray.origin_ - vertices_[1];
	glm::vec3 Q = glm::cross(T, edge1);

	float u = glm::dot(T, P) * invDet;
	float v = glm::dot(ray.direction_, Q) * invDet;

	if (u < 0.0f || u > 1.0f || v < 0.0f || v > 1.0f) {
		return false;
	}

	t = glm::dot(edge2, Q) * invDet;

	if (t > FLT_EPSILON) {
		ray.setEnd(t);
		ray.intersectionNormal_ = getNormal(ray.end_);
		return true;
	}
	return false;
}

glm::vec3 Rectangle::getPosition()
{
	return (vertices_[0] + vertices_[1]) / 2.0f + (vertices_[1] + vertices_[2]) / 2.0f;
}

glm::vec3 Rectangle::getNormal(const glm::vec3& intersectionPoint)
{
	glm::vec3 edge1 = glm::normalize(vertices_[0] - vertices_[1]);
	glm::vec3 edge2 = glm::normalize(vertices_[2] - vertices_[1]);
	normal_ = glm::normalize(glm::cross(edge1, edge2));
	return normal_;
}

float Rectangle::getArea()
{
	float edge1 = glm::length(vertices_[1] - vertices_[0]);
	float edge2 = glm::length(vertices_[2] - vertices_[0]);
	return edge1 * edge2;
}

glm::vec3 Rectangle::getPointOnSurface(const glm::vec3& normal, float u, float v)
{
	glm::vec3 edge1 = glm::normalize(vertices_[0] - vertices_[1]);
	glm::vec3 edge2 = glm::normalize(vertices_[2] - vertices_[1]);

	return vertices_[1] + edge1 * u + edge2 * v;
}

void Rectangle::GUI()
{
	ImGui::SliderFloat3("Vertex 1", &vertices_[0].x, -10.0f, 10.0f);
	ImGui::SliderFloat3("Vertex 2", &vertices_[1].x, -10.0f, 10.0f);
	ImGui::SliderFloat3("Vertex 3", &vertices_[2].x, -10.0f, 10.0f);
	area_ = getArea();
	ImGui::NewLine();
}
