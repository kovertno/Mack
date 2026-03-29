#pragma once
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "Camera.hpp"

class TerrainCollisionSystem {
public:
	void bake(entt::registry& registry);
	void update(entt::registry& registry, float deltaTime);
	void updateCamera(Camera& camera, entt::registry& registry);

private:
	bool rayTriangleIntersect(const glm::vec3& orig, const glm::vec3& dir, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, float& outT);
};
