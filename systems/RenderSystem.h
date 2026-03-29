#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include "Shader.hpp"
#include "Camera.hpp"
#include "SceneShaders.hpp"

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
#include "OutlineComponent.hpp"
#include "Framebuffer.hpp"

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
	
	static void RenderBoxes(entt::registry& registry, Shader* cubeShader, Shader* outlineShader);
	static void RenderFloor(entt::registry& registry, Shader* shader);
	static void RenderCrosshair(entt::registry& registry, Shader* shader);
	static void RenderGrass(entt::registry& registry, Shader* shader);
	static void RenderTree(entt::registry& registry, Shader* modelShader, Shader* outlineShader);
	static void RenderTrunk(entt::registry& registry, Shader* modelShader, Shader* outlineShader);
	static void RenderRock(entt::registry& registry, Shader* modelShader, Shader* outlineShader);
	static void RenderBush(entt::registry& registry, Shader* modelShader, Shader* outlineShader);
	static void RenderMushroom(entt::registry& registry, Shader* modelShader, Shader* outlineShader);
	static void RenderPostProcessing(Shader* shader, unsigned int VAO, unsigned int textureAttachment);
	static void RenderSkybox(Shader* shader, unsigned int VAO, unsigned int skyboxTexture);
	static void RenderFlashlight(entt::registry& registry, Shader* shader, std::unique_ptr<Camera>& camera);
	static void RenderScene(entt::registry& registry, SceneShaders& sceneShaders, unsigned int skyboxVAO, unsigned int skyboxTexture);
	static void RenderSystem::renderMapModel(entt::registry& registry, Shader* modelShader, Shader* outlineShader);
};

#endif