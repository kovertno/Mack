#include "FireflySystem.h"

#include "TransformComponent.hpp"
#include "ModelMeshComponent.hpp"
#include "FireflyModelComponent.hpp"

#include <GLFW/glfw3.h>
#include <entt/entt.hpp>

void FireflySystem::Update(entt::registry& registry) {
	auto view = registry.view<TransformComponent, ModelMeshComponent, FireflyModelComponent>();
	for (auto entity : view) {
		auto& transform = registry.get<TransformComponent>(entity);
		auto& component = registry.get<FireflyModelComponent>(entity);

		float radius = 1.25f;

		transform.position.x = component.startingPos.x + radius * cos(static_cast<float>(glfwGetTime()) + component.offset);
		transform.position.y = component.startingPos.y * ((sin(static_cast<float>(glfwGetTime()) + component.offset) * 2.0f) / 16.0f) + 0.5f;
		transform.position.z = component.startingPos.z + radius * sin(static_cast<float>(glfwGetTime()) + component.offset);

		transform.rotation.y = -glm::degrees(static_cast<float>(glfwGetTime()) + component.offset) - 90.0f;
	}
}