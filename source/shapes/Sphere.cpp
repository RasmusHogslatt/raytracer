//#pragma once
//
//#include <shapes/Sphere.h>
//
//Sphere::Sphere(float radius) : radius{radius}
//{
//}
//
//bool Sphere::intersect(Ray& ray, float* tHit) const
//{
//	glm::vec3 l = ray.origin.position - ray.direction;
//	float tca = glm::dot(l, ray.direction);
//	float d2 = glm::dot(l, l) - tca * tca;
//	if (d2 > radius * radius) {
//		return false;
//	}
//	float thc = sqrt(radius * radius - d2);
//	*tHit = tca - thc;
//	return true;
//}