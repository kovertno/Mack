#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <limits>

class RayCastSystem {
public:
	static bool RayVsBox(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& boxMin, const glm::vec3& boxMax, float& outDistance);
	static entt::entity RayCast(entt::registry& registry, const glm::vec3& rayOrigin, const glm::vec3& rayDir, float maxDistance = 5.0f);
};