#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "Shader.hpp"
#include "Camera.hpp"
#include "Game.hpp"

#include "TransformComponent.hpp"
#include "BoxMeshComponent.hpp"

#include <entt/entt.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class RenderSystem {
private:
	
public:
	RenderSystem() = default;
	~RenderSystem() = default;

	static void Render(entt::registry& registry, Shader* shader, std::unique_ptr<Camera>& camera);
};

#endif