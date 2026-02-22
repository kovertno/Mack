#include "PhysicsSystem.h"
#include "../components/RigidBodyComponent.h"
#include "../components/GravityComponent.h"

void PhysicsSystem::update(entt::registry& registry, float deltaTime)
{
	auto gravityView = registry.view<GravityComponent, RigidBodyComponent>();
	gravityView.each([deltaTime](auto& gravity, auto& rigidBody)
		{
			if (!rigidBody.isStatic)
			{
				rigidBody.acceleration += gravity.gravityPull;
				rigidBody.velocity += rigidBody.acceleratio * deltaTime);
				rigidBody.acceleration = glm::vec3(0.0f); //reset it after every frame to make the fall const
			}
			else
			{
				return;
			}
		});
}