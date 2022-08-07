#pragma once

#include "Camera.h"


Camera::Camera(glm::vec3 position, glm::vec2 resolution, float fov, glm::vec3 up, glm::vec3 front) : position{position}, up{up}, fov{fov}, resolution{resolution}, front{front}, yaw{0}, pitch{0}
{
	worldUp = up;
    updateVectors();
}

glm::vec3 Camera::generateRay(int pixelX, int pixelY)
{
    glm::mat4 cameraToWorld;
   
    float scale = tan(glm::radians(fov * 0.5));
    float aspectRatio = static_cast<float>(resolution.x ) / static_cast<float>(resolution.y);
    // x and y is center of pixel sample
    float x = (2 * (pixelX + 0.5) / (float)resolution.x - 1) * aspectRatio * scale;
    float y = (1 - 2 * (pixelY + 0.5) / (float)resolution.y) * scale;
    glm::vec3 pixelInPlanePosition = glm::normalize(glm::vec3(x, y, -1));
    glm::vec3 dir = glm::normalize(pixelInPlanePosition - position);
    return dir;
}

void Camera::updateVectors()
{
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}