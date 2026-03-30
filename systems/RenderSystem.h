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
	static void RenderPostProcessing(Shader* shader, unsigned int VAO, unsigned int textureAttachment);
	static void RenderSkybox(Shader* shader, unsigned int VAO, unsigned int skyboxTexture);
	static void RenderFlashlight(entt::registry& registry, Shader* shader, std::unique_ptr<Camera>& camera);
	static void RenderFireflies(entt::registry& registry, Shader* shader);
	static void RenderScene(entt::registry& registry, SceneShaders& sceneShaders, unsigned int skyboxVAO, unsigned int skyboxTexture);

	template<typename TagComponent>
	static void RenderSceneModel(entt::registry& registry, Shader* modelShader, Shader* outlineShader) {
		auto view = registry.view<TransformComponent, ModelMeshComponent, TagComponent>();
		for (auto entity : view) {
			auto& transform = view.get<TransformComponent>(entity);
			auto& mesh = view.get<ModelMeshComponent>(entity);

			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);
			glClear(GL_STENCIL_BUFFER_BIT);

			modelShader->Use();
			modelShader->SetMat4("model", transform.GetModelMatrix());
			modelShader->SetFloat("material.shininess", 32.0f);
			mesh.model.Draw(modelShader);

			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);

			outlineShader->Use();
			outlineShader->SetMat4("model", transform.GetModelMatrix());
			mesh.model.Draw(outlineShader);
		}
	}
};

#endif