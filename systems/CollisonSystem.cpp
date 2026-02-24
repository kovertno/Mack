#include "CollisionSystem.hpp"
#include <iostream>
#include "../components/TransformComponent.hpp"
#include "../components/RigidBodyComponent.h"
#include "../components/BoxColliderComponent.hpp"


static inline glm::vec3 worldHalfExtents(const TransformComponent& transform, const BoxColliderComponent& collider) {
	return transform.scale * collider.halfExtents;
}

void CollisionSystem::update(entt::registry& registry, float deltaTime){

    auto dynamics = registry.view<TransformComponent, BoxColliderComponent, RigidBodyComponent>();
    auto statics = registry.view<TransformComponent, BoxColliderComponent, RigidBodyComponent>();

    for (auto dynamicEntity : dynamics){

        auto& dynamicTransform = dynamics.get<TransformComponent>(dynamicEntity);
        auto& dynamicCollider = dynamics.get<BoxColliderComponent>(dynamicEntity);
        auto& dynamicRigidBody = dynamics.get<RigidBodyComponent>(dynamicEntity);
        glm::vec3 VelOfObject = dynamicRigidBody.velocity;

        if (dynamicRigidBody.isStatic)
            continue;

        const glm::vec3 dynamicsHalfExtents = worldHalfExtents(dynamicTransform, dynamicCollider);

        for (auto staticEntity : statics){

            auto& staticTransform = statics.get<TransformComponent>(staticEntity);
            auto& staticCollider = statics.get<BoxColliderComponent>(staticEntity);
            auto& staticRigidBody = statics.get<RigidBodyComponent>(staticEntity);

            if (!staticRigidBody.isStatic)
                continue;

            if (dynamicEntity == staticEntity)
                continue;

            const glm::vec3 staticHalfExtents = worldHalfExtents(staticTransform, staticCollider);

            // AABB collision detection
            glm::vec3 worldPosDiff = dynamicTransform.position - staticTransform.position;

            glm::vec3 overlaps = (dynamicsHalfExtents + staticHalfExtents) - glm::abs(worldPosDiff);

            if (overlaps.x > 0.0f && overlaps.y > 0.0f && overlaps.z > 0.0f){
                if (worldPosDiff.y < 0.0f)
                    continue;

                dynamicTransform.position.y = staticTransform.position.y + staticHalfExtents.y + dynamicsHalfExtents.y;

                if (dynamicRigidBody.velocity.y < 0.0f){
                    dynamicRigidBody.velocity.y = 0.0f;
                    dynamicRigidBody.velocity.x *= 0.99f;
                    dynamicRigidBody.velocity.z *= 0.99f;
                    if (glm::abs(dynamicRigidBody.velocity.x) < 0.01f)
                        dynamicRigidBody.velocity.x = 0.0f;
                    if (glm::abs(dynamicRigidBody.velocity.z) < 0.01f)
                        dynamicRigidBody.velocity.z = 0.0f;
                }
                else {
                    dynamicRigidBody.velocity = VelOfObject;
                }
                //dynamicRigidBody.isStatic = true;
            }
        }
    }
}

