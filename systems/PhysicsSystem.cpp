#include <iostream>
#include "PhysicsSystem.h"
#include "RigidBodyComponent.h"
#include "GravityComponent.h"
#include "TransformComponent.hpp"

void PhysicsSystem::update(entt::registry& registry, float deltaTime)
{
	auto gravityView = registry.view<TransformComponent, GravityComponent, RigidBodyComponent>();
	gravityView.each([deltaTime](auto& transform ,auto& gravity, auto& rigidBody)
		{
			if (!rigidBody.isStatic)
			{
				rigidBody.acceleration += gravity.gravityPull;
				rigidBody.velocity += rigidBody.acceleration * deltaTime;
				transform.position += rigidBody.velocity * deltaTime;
				rigidBody.acceleration = glm::vec3(0.0f); //reset it after every frame to make the fall const
			}
			else
			{
				return;
			}
		});
}
