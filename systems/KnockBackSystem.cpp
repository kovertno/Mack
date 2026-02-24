#include "KnockBackSystem.hpp"
#include "TransformComponent.hpp"

void KnockBackSystem::ApplyKnockBack(entt::registry& registry, entt::entity entity, const glm::vec3& direction, float force) {
	if (!registry.all_of<TransformComponent,KnockBackComponent, RigidBodyComponent>(entity)) return;

	auto& rigidbody = registry.get<RigidBodyComponent>(entity);
	auto& transform = registry.get<TransformComponent>(entity);
	rigidbody.velocity += direction * force;
	rigidbody.velocity.y += 1.0f;
	//transform.position = rigidbody.velocity;

}