#pragma once

#include "Object.h"
#include <glm/glm.hpp>

Object::Object(const glm::vec3& position, const glm::vec3& normal) : position_{ position }, normal_{ normal } {}

Sphere::Sphere(const glm::vec3& position, const glm::vec3& normal, float radius) : Object(position, normal), radius_{ radius }, radius2_{radius_*radius_} {}

bool Sphere::intersect(const glm::vec3& origo, const glm::vec3& direction, float& t0, float& t1)
{
    glm::vec3 l = position_ - origo;
    float tca = glm::dot(direction, l);
    if (tca < 0)
    {
        return false;
    }
    float d2 = glm::dot(l,l) - tca * tca;
    if (d2 > radius2_) return false;
    float thc = sqrt(radius_*radius_ - d2);
    t0 = tca - thc;
    t1 = tca + thc;

    return true;
}
