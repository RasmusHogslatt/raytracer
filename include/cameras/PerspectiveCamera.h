//#pragma once
//
//#include <cameras/Camera.h>
//
//class PerspectiveCamera : public ProjectiveCamera {
//public:
//	PerspectiveCamera(float lensRadius, float focalLength, float fov, Texture* texture);
//
//	float generateRay(const CameraSample& sample, Ray& ray) const;
//
//private:
//	glm::vec3 dxCamera, dyCamera;
//	float a;
//};