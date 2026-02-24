#include "RayCastSystem.hpp"
#include <iostream>
#include "TransformComponent.hpp"
#include "BoxColliderComponent.hpp"
#include <entt/entt.hpp>

// Ray vs Axis-Aligned Bounding Box (AABB) intersection test.
// Uses the "slab method": compute intersection intervals per axis (X, Y, Z)
// and check whether the intervals overlap.
// If they do, the ray hits the box and outDistance is set to the
// nearest intersection distance along the ray.

bool RayCastSystem::RayVsBox(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& boxMin, const glm::vec3& boxMax, float& outDistance){
    // --- X slab ---
    float targetXMin = (boxMin.x - rayOrigin.x) / rayDir.x;
    float targetXMax = (boxMax.x - rayOrigin.x) / rayDir.x;
    if (targetXMin > targetXMax) std::swap(targetXMin, targetXMax);

    // --- Y slab ---
    float targetYMin = (boxMin.y - rayOrigin.y) / rayDir.y;
    float targetYMax = (boxMax.y - rayOrigin.y) / rayDir.y;
    if (targetYMin > targetYMax) std::swap(targetYMin, targetYMax);

    // Check overlap between X and Y intervals
    if ((targetXMin > targetYMax) || (targetYMin > targetXMax))
        return false;

    // Narrow interval to intersection of X and Y
    if (targetYMin > targetXMin) targetXMin = targetYMin;
    if (targetYMax > targetXMax) targetXMax = targetYMax;

    // --- Z slab ---
    float targetZMin = (boxMin.z - rayOrigin.z) / rayDir.z;
    float targetZMax = (boxMax.z - rayOrigin.z) / rayDir.z;
    if (targetZMin > targetZMax) std::swap(targetZMin, targetZMax);

    // Check overlap with accumulated interval
    if ((targetXMin > targetZMax) || (targetZMin > targetXMax))
        return false;

    // Final entry distance
    if (targetZMin > targetXMin) targetXMin = targetZMin;

    outDistance = targetXMin;

    // Intersection must be in front of the ray
    return targetXMin >= 0.0f;
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

        glm::vec3 halfSize = transform.scale * collider.halfExtents;
        glm::vec3 boxMin = transform.position - halfSize;
        glm::vec3 boxMax = transform.position + halfSize;

        float hitDistance;
        if (RayVsBox(rayOrigin, rayDir, boxMin, boxMax, hitDistance)) {
            if (hitDistance < closestDistance && hitDistance <= maxDistance) {
                closestDistance = hitDistance;
                hitEntity = entity;
                std::cout << "hit";
            }
        }
    }
    return hitEntity;
}