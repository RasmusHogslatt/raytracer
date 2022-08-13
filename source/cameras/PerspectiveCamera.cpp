//#pragma once
//
//#include <cameras/PerspectiveCamera.h>
//#include <cameras/Camera.h>
//
//Camera::Camera(Texture* texture) : texture{ texture }
//{
//}
//
//Camera::~Camera()
//{
//
//}
//
//ProjectiveCamera::ProjectiveCamera(float lensradius, float focallength) : Camera(), lensRadius{ lensradius }, focalLength{ focallength }
//{
//}
//
//PerspectiveCamera::PerspectiveCamera(float lensRadius, float focalLength, float fov, Texture* texture) : lensRadius{ lensRadius }, focalLength{ focalLength }, texture{ texture } {
//	glm::vec2 res = glm::vec2(texture->getWidth(), texture->getHeight());
//	glm::vec2 pMin = glm::vec2(0, 0);
//	glm::vec2 pMax = res;
//}
//
//float PerspectiveCamera::generateRay(const CameraSample& sample, Ray& ray) const
//{
//	
//}
