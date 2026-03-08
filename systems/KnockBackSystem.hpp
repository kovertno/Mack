#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "KnockBackComponent.hpp"
#include "RigidBodyComponent.h"

class KnockBackSystem {
public:
	static void ApplyKnockBack(entt::registry& registry, entt::entity entity, const glm::vec3& direction, float force);
};