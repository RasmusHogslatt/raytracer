//#pragma once
//
//#include "Camera.h"
//#include <glm/gtc/matrix_transform.hpp>
//
//Camera::Camera(glm::vec3 position, glm::vec3 up, glm::vec3 front, float yaw, float pitch) : position{ position }, up{ up }, front{ front }, yaw{ yaw }, pitch{ pitch }, right{ glm::vec3(1, 0, 0) }
//{
//
//	worldUp = glm::vec3(0, 1, 0);
//	//updateCameraVectors();
//}
//
//void Camera::updateCameraVectors()
//{
//	glm::vec3 front = glm::vec3(0);
//	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
//	front.y = sin(glm::radians(pitch));
//	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
//	front = glm::normalize(front);
//	right = glm::normalize(glm::cross(front, worldUp));
//	up = glm::normalize(glm::cross(right, front));
//}
//
//glm::mat4 Camera::getViewMatrix()
//{
//	return glm::lookAt(position, position + front, up);
//}
