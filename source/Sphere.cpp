#include "Sphere.h"

Sphere::Sphere(float radius) : radius_ { radius } {
}

bool Sphere::intersect(const glm::vec3& rayOrigin, const glm::vec3& rayDirection)
{
	glm::vec3 l = position_ - rayOrigin;
	float tca = glm::dot(l, rayDirection);
	float d2 = glm::dot(l, l) - tca * tca;
	if (d2 > radius_ * radius_) {
		return false;
	}
	float thc = sqrt(radius_ * radius_ - d2);
	
	return true;
}
