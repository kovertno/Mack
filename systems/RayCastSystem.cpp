#include "RayCastSystem.hpp"
#include "TransformComponent.hpp"
#include <iostream>
#include "BoxColliderComponent.hpp"
#include "RigidBodyComponent.h"
#include <entt/entt.hpp>

// Ray vs Axis-Aligned Bounding Box (AABB) intersection test.
// Uses the "slab method": compute intersection intervals per axis (X, Y, Z)
// and check whether the intervals overlap.
// If they do, the ray hits the box and outDistance is set to the
// nearest intersection distance along the ray.

bool RayCastSystem::RayVsBox(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& boxMin, const glm::vec3& boxMax, float& outDistance) {
    // Use inverse direction to avoid division by zero and improve performance
    glm::vec3 invDir;
    invDir.x = (rayDir.x != 0.0f) ? 1.0f / rayDir.x : std::numeric_limits<float>::max();
    invDir.y = (rayDir.y != 0.0f) ? 1.0f / rayDir.y : std::numeric_limits<float>::max();
    invDir.z = (rayDir.z != 0.0f) ? 1.0f / rayDir.z : std::numeric_limits<float>::max();

    // Compute intersection distances for each axis slab
    float targetXMin = (boxMin.x - rayOrigin.x) * invDir.x;
    float targetXMax = (boxMax.x - rayOrigin.x) * invDir.x;
    if (targetXMin > targetXMax) std::swap(targetXMin, targetXMax); // ensure min < max

    float targetYMin = (boxMin.y - rayOrigin.y) * invDir.y;
    float targetYMax = (boxMax.y - rayOrigin.y) * invDir.y;
    if (targetYMin > targetYMax) std::swap(targetYMin, targetYMax);

    // If intervals don't overlap, ray misses the box
    if ((targetXMin > targetYMax) || (targetYMin > targetXMax))
        return false;

    // Narrow to the overlap interval
    if (targetYMin > targetXMin) targetXMin = targetYMin;
    if (targetYMax < targetXMax) targetXMax = targetYMax;

    float targetZMin = (boxMin.z - rayOrigin.z) * invDir.z;
    float targetZMax = (boxMax.z - rayOrigin.z) * invDir.z;
    if (targetZMin > targetZMax) std::swap(targetZMin, targetZMax);

    // Final overlap check with Z slab
    if ((targetXMin > targetZMax) || (targetZMin > targetXMax))
        return false;

    // Entry point is the furthest near intersection
    if (targetZMin > targetXMin) targetXMin = targetZMin;

    outDistance = targetXMin;
    return targetXMin >= 0.0f; // negative means box is behind the ray
}

// returns the entity that was hit
entt::entity RayCastSystem::RayCast(entt::registry& registry, const glm::vec3& rayOrigin, const glm::vec3& rayDir, float maxDistance) {
    entt::entity hitEntity = entt::null;
    float closestDistance = std::numeric_limits<float>::max();

    //check all the entities that have boxColliderComponent and transformComponent
    auto view = registry.view<TransformComponent, BoxColliderComponent>();
    for (auto entity : view) {
        auto& transform = view.get<TransformComponent>(entity);
        auto& collider = view.get<BoxColliderComponent>(entity);

        if (registry.all_of<RigidBodyComponent>(entity)) {
            auto& ignoreStaticRigidbody = registry.get<RigidBodyComponent>(entity);
            if (ignoreStaticRigidbody.isStatic) continue;
        }

        glm::vec3 halfSize = transform.scale * collider.halfExtents;
        glm::vec3 boxMin = transform.position - halfSize;
        glm::vec3 boxMax = transform.position + halfSize;

        float hitDistance;
        if (RayVsBox(rayOrigin, rayDir, boxMin, boxMax, hitDistance)) {
            if (hitDistance < closestDistance && hitDistance <= maxDistance) {
                closestDistance = hitDistance;
                hitEntity = entity;

                /*std::cout << "hit entity: " << (uint32_t)entity
                    << " distance: " << hitDistance
                    << " pos: " << transform.position.x << " " << transform.position.y << " " << transform.position.z
                    << " halfSize: " << halfSize.x << " " << halfSize.y << " " << halfSize.z << "\n";*/
            }
        }
    }
    return hitEntity;
}