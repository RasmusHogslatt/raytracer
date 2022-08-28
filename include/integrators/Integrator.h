#pragma once

#include <Scene.h>

class Integrator {
public:
	Integrator(Scene* scene);
	virtual void Render() = 0;
	virtual void GUISettings();

	// Offsets an intersection point with a bias towards normal's direction
	void offsetIntersectionPoint(Ray& offsetRay, const glm::vec3& normal, float bias);

	// Takes ray without offset and normal. Returns reflected ray with offset origin
	Ray& getReflectionRay(const Ray& ray, const glm::vec3& normal);
	// Takes ray without offset, normal and ior. Returns refracted ray with offset origin
	Ray& getRefractionRay(const Ray& ray, const glm::vec3& normal, float ior);

	glm::vec3 phongShading(Object* object, const glm::vec3& normal);

	Material& getMaterial(Object* object);

	// Returns index for closest intersected object in scene.primitives vector. 
	// Returns -1 if no was intersection found
	int getClosestObjectIndex(const Ray& ray);

public:
	int maxRecursiveDepth;
	int currentRecurseDepth;
	float bias;
	float globalIOR;
	Scene* scene;
};