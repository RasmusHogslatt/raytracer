#pragma once

#include "Actor.h"
#include <limits>

Actor::Actor(const glm::vec3& pos) : position_{ pos } {
}

Actor::Actor(const Actor& old)
{
	position_ = old.position_;

}
