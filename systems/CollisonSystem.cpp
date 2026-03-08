#include "CollisionSystem.hpp"
#include <iostream>
#include "TransformComponent.hpp"
#include "RigidBodyComponent.h"
#include "BoxColliderComponent.hpp"
#include "StaticColliderComponent.hpp"


static inline glm::vec3 worldHalfExtents(const TransformComponent& transform, const BoxColliderComponent& collider) {
	return transform.scale * collider.halfExtents;
}

void CollisionSystem::update(entt::registry& registry, float deltaTime){

    //dynamic objects
    auto dynamics = registry.view<TransformComponent, BoxColliderComponent, RigidBodyComponent>();
    //static objects
    auto statics = registry.view<TransformComponent, BoxColliderComponent, StaticColliderComponent>();

    for (auto dynamicEntity : dynamics){

        auto& dynamicTransform = dynamics.get<TransformComponent>(dynamicEntity);
        auto& dynamicCollider = dynamics.get<BoxColliderComponent>(dynamicEntity);
        auto& dynamicRigidBody = dynamics.get<RigidBodyComponent>(dynamicEntity);

        if (dynamicRigidBody.isStatic)
            continue;

        const glm::vec3 dynamicsHalfExtents = worldHalfExtents(dynamicTransform, dynamicCollider);

        // Dynamic vs Static
        for (auto staticEntity : statics){

            auto& staticTransform = statics.get<TransformComponent>(staticEntity);
            auto& staticCollider = statics.get<BoxColliderComponent>(staticEntity);

            //Can't happen since static entites don't have RigidBodyComponent but just in case if somthing very wrong happens we leave it untouched
            if (dynamicEntity == staticEntity)
                continue;

            const glm::vec3 staticHalfExtents = worldHalfExtents(staticTransform, staticCollider);

            // AABB collision detection
            glm::vec3 worldPosDiff = dynamicTransform.position - staticTransform.position;
            glm::vec3 overlaps = (dynamicsHalfExtents + staticHalfExtents) - glm::abs(worldPosDiff);

            // no collision
            if (overlaps.x <= 0.0f || overlaps.y <= 0.0f || overlaps.z <= 0.0f){
                continue;
            }

            //find min overlap axis(minimum translation vector)
            glm::vec3 mtv(0.0f);
            float minOverlap = std::numeric_limits<float>::max();

            if (overlaps.x < minOverlap) {
                minOverlap = overlaps.x; mtv = glm::vec3(1, 0, 0);
            }
            if (overlaps.y < minOverlap) {
                minOverlap = overlaps.y; mtv = glm::vec3(0, 1, 0);
            }
            if (overlaps.z < minOverlap) {
                minOverlap = overlaps.z; mtv = glm::vec3(0, 0, 1);
            }

            // ensure mtv points from static toward dynamic (away form surafce)
            if (glm::dot(mtv, worldPosDiff) < 0.0f) {
                mtv = -mtv;
            }

            //push dynamic objects out of static geometry
            dynamicTransform.position += mtv * minOverlap;

            // only repsond if the object is moving into/thorugh suraface
            float velAlongMTV = glm::dot(dynamicRigidBody.velocity, mtv);
            if (velAlongMTV < 0.0f) {

                // cancel velocity along collision axis
                dynamicRigidBody.velocity -= velAlongMTV * mtv;

                //Apply friction to velocity perpendicular to the collsion axis(sliding)
                glm::vec3 tangentialVel = dynamicRigidBody.velocity - glm::dot(dynamicRigidBody.velocity, mtv) * mtv;
                tangentialVel *= 0.995f; // 0 - objects stops after landing(no slidng) 1 - perfectly smooth surace object wont stop

                //make sure the object wont slide infinitly
                if (glm::abs(tangentialVel.x) < 0.01f) tangentialVel.x = 0.0f;
                if (glm::abs(tangentialVel.y) < 0.01f) tangentialVel.y = 0.0f;
                if (glm::abs(tangentialVel.z) < 0.01f) tangentialVel.z = 0.0f;
                dynamicRigidBody.velocity = tangentialVel;
                
            }
        }
        for (auto otherEntity : dynamics) {

            // ensure that each pair is processed only once
            if (dynamicEntity >= otherEntity) {
                continue;
            }
            auto& otherTransform = dynamics.get<TransformComponent>(otherEntity);
            auto& otherCollider = dynamics.get<BoxColliderComponent>(otherEntity);
            auto& otherRigidBody = dynamics.get<RigidBodyComponent>(otherEntity);

            if (otherRigidBody.isStatic) {
                continue;
            }

            const glm::vec3 otherHalfExtent = worldHalfExtents(otherTransform, otherCollider);

            // AABB collision detection
            glm::vec3 worldPosDiff = dynamicTransform.position - otherTransform.position;
            glm::vec3 overlaps = (dynamicsHalfExtents + otherHalfExtent) - glm::abs(worldPosDiff);

            //no collision
            if (overlaps.x <= 0.0f || overlaps.y <= 0.0f || overlaps.z <= 0.0f) {
                continue;
            }

            //find min overlap axis(minimum translation vector)
            glm::vec3 mtv(0.0f);
            float minOverlap = std::numeric_limits<float>::max();

            if (overlaps.x < minOverlap) {
                minOverlap = overlaps.x; mtv = glm::vec3(1, 0, 0);
            }
            if (overlaps.y < minOverlap) {
                minOverlap = overlaps.y; mtv = glm::vec3(0, 1, 0);
            }
            if (overlaps.z < minOverlap) {
                minOverlap = overlaps.z; mtv = glm::vec3(0, 0, 1);
            }

            // ensure mtv points form other towardd dynamic
            if (glm::dot(mtv, worldPosDiff) < 0.0f) {
                mtv = -mtv;
            }

            float massA = dynamicRigidBody.mass;
            float massB = otherRigidBody.mass;
            float totalMass = massA + massB;

            // Separate objects proportionally to their mass (heavier moves less)
            dynamicTransform.position += mtv * minOverlap * (massB / totalMass);
            otherTransform.position -= mtv * minOverlap * (massA / totalMass);

            // Compute impulse for elastic collision along the MTV axis
            // restitution: 0 = perfectly inelastic, 1 = perfectly elastic
            float restitution = 0.5f;
            float velA = glm::dot(dynamicRigidBody.velocity, mtv);
            float velB = glm::dot(otherRigidBody.velocity, mtv);

            float impulse = (-(1.0f + restitution) * (velA - velB)) / (1.0f / massA + 1.0f / massB);

            // Apply impulse to both objects in opposite directions
            dynamicRigidBody.velocity += (impulse / massA) * mtv;
            otherRigidBody.velocity -= (impulse / massB) * mtv;
        }
    }
}

