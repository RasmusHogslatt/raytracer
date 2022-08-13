//#pragma once
//
//#include <glm/glm.hpp>
//#include <Texture.h>
//#include <Ray.h>
//
//struct CameraSample {
//	glm::vec2 filmPosition;
//	glm::vec2 lensposition;
//};
//
//class Camera {
//public:
//	Camera() = default;
//	Camera(Texture* texture);
//	virtual ~Camera();
//	virtual float generateRay(const CameraSample& sample, Ray& ray) const = 0;
//
//	Texture* texture;
//};
//
//class ProjectiveCamera : public Camera {
//public:
//	ProjectiveCamera(float lensradius, float focallength);
//	virtual float generateRay(const CameraSample& sample, Ray& ray) const = 0;
//protected:
//	float lensRadius, focalLength;
//};