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
#include "ModelMeshComponent.hpp"
#include "TreeModelComponent.hpp"
#include "TrunkModelComponent.hpp"
#include "RockModelComponent.hpp"
#include "BushModelComponent.hpp"
#include "MushroomModelComponent.hpp"

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
	static void SetBackpackStaticUniforms(Shader* shader);
	static void SetBackpackDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera);
	static void RenderBackpack(entt::registry& registry, Shader* shader);
	static void SetTreeStaticUniforms(Shader* shader);
	static void SetTreeDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera);
	static void RenderTree(entt::registry& registry, Shader* shader);
	static void SetTrunkStaticUniforms(Shader* shader);
	static void SetTrunkDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera);
	static void RenderTrunk(entt::registry& registry, Shader* shader);
	static void SetRockStaticUniforms(Shader* shader);
	static void SetRockDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera);
	static void RenderRock(entt::registry& registry, Shader* shader);
	static void SetBushStaticUniforms(Shader* shader);
	static void SetBushDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera);
	static void RenderBush(entt::registry& registry, Shader* shader);
	static void SetMushroomStaticUniforms(Shader* shader);
	static void SetMushroomDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera);
	static void RenderMushroom(entt::registry& registry, Shader* shader);
};

#endif