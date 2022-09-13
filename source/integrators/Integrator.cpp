#pragma once

#include <integrators/Integrator.h>

Integrator::Integrator(Scene* scene) : scene_{ scene } {}

glm::vec3 Integrator::getReflectionDirection(const glm::vec3& incident, const glm::vec3& normal) const
{
    return glm::normalize(incident - 2.0f * glm::dot(incident, normal) * normal);
}
