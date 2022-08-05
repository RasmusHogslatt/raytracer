//#pragma once
//
//#include <glm/glm.hpp>
//
//class Camera {
//public:
//	glm::vec3 position;
//	glm::vec3 front;
//	glm::vec3 up;
//	glm::vec3 right;
//	glm::vec3 worldUp;
//	float yaw, pitch;
//	
//	Camera(glm::vec3 position = glm::vec3(0), glm::vec3 up = glm::vec3(0,1,0), glm::vec3 front = glm::vec3(0, 0, -1), float yaw = 0, float pitch = 0);
//
//	void updateCameraVectors();
//	glm::mat4 getViewMatrix();
//
//};