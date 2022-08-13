//#pragma once
//
//#include "Primitive.h"
//
//Primitive::~Primitive() {}
//
//GeometricPrimitive::GeometricPrimitive(const std::shared_ptr<Shape>& shape, const std::shared_ptr<Material>& material) : shape{ shape }, material{ material } {}
//
//bool GeometricPrimitive::intersect(Ray& ray) const
//{
//	float tHit;
//	if (!shape->intersect(ray, &tHit)) {
//		return false;
//	}
//	ray.tMax = tHit;
//	return true;
//}
//
//const Material* GeometricPrimitive::GetMaterial() const
//{
//	return material.get();
//}
