#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "Shader.hpp"
#include "Camera.hpp"
#include "Game.hpp"

#include "TransformComponent.hpp"
#include "BoxMeshComponent.hpp"
#include "CrosshairMeshComponent.hpp"
#include "MaterialComponent.hpp"
#include "GrassMeshComponent.hpp"

#include <entt/entt.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class RenderSystem {
private:
	
public:
	RenderSystem() = default;
	~RenderSystem() = default;
	
	static void SetDirectionalLightUniforms(Shader* shader);
	static void SetBoxStaticUniforms(Shader* shader);
	static void SetBoxDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera);
	static void RenderBoxes(entt::registry& registry, Shader* shader);
	static void SetCrosshairStaticUniforms(Shader* shader);
	static void RenderCrosshair(entt::registry& registry, Shader* shader);
	static void SetGrassStaticUniforms(Shader* shader);
	static void SetGrassDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera);
	static void RenderGrass(entt::registry& registry, Shader* shader);
};

#endif