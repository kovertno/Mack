#pragma once
#include <entt/entt.hpp>

class PhysicsSystem
{
public:
	void update(entt::registry& registry, float deltaTime);
};