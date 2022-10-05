#pragma once

#include <glm/glm.hpp>
#include <materials/Material.h>
#include <memory>

class Ray {
public:
	Ray(const glm::vec3& o = glm::vec3(0), const glm::vec3& d = glm::vec3(0), std::shared_ptr<Ray> parent = nullptr);
	~Ray();
	void setEnd(float t);
public:
	glm::vec3 origin_;
	glm::vec3 direction_;
	glm::vec3 end_;
	glm::vec3 importance_;
	glm::vec3 radiance_;
	glm::vec3 intersectionNormal_;
	glm::vec3 orthoToNormal_;
	Material* material_;
	int lightIndex_;
	bool inside_;
	std::shared_ptr<Ray> parent_;
	std::shared_ptr<Ray> child_;
};