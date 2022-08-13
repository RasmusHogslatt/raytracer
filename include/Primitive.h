//#pragma once
//
//#include <glm/glm.hpp>
//#include <materials/Material.h>
//#include <memory>
//#include <lights/Light.h>
//#include <shapes/Shape.h>
//#include <Ray.h>
//
//class Primitive {
//public:
//	virtual ~Primitive();
//	virtual bool intersect(Ray& ray) const = 0;
//	virtual const Material* GetMaterial() const = 0;
//};
//
//class GeometricPrimitive : public Primitive {
//public:
//	GeometricPrimitive(const std::shared_ptr<Shape>& shape, const std::shared_ptr<Material>& material);
//	virtual bool intersect(Ray& ray) const;
//	virtual const Material* GetMaterial() const;
//private:
//	std::shared_ptr<Shape> shape;
//	std::shared_ptr<Material> material;
//};