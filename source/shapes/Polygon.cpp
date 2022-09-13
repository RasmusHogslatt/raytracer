#pragma once

#include <shapes/Polygon.h>

Polygon::Polygon() : vertices_{ std::vector<glm::vec3>() }, normal_{glm::normalize(glm::vec3(1))}
{

}

Polygon::~Polygon()
{
	
}