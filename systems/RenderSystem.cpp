#include "Shader.hpp"
#include "Camera.hpp"

#include "RenderSystem.h"
#include "TransformComponent.hpp"
#include "BoxMeshComponent.hpp"
#include "CrosshairMeshComponent.hpp"
#include "MaterialComponent.hpp"
#include "GrassMeshComponent.hpp"
#include "TreeModelComponent.hpp"
#include "TrunkModelComponent.hpp"
#include "RockModelComponent.hpp"
#include "BushModelComponent.hpp"
#include "MushroomModelComponent.hpp"
#include "OutlineComponent.hpp"
#include "Framebuffer.hpp"
#include "SceneShaders.hpp"

#include <entt/entt.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void RenderSystem::RenderBoxes(entt::registry& registry, Shader* cubeShader, Shader* outlineShader) {
    auto view1 = registry.view<TransformComponent, BoxMeshComponent, MaterialComponent, OutlineComponent>();
    for (auto& entity : view1) {
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        glClear(GL_STENCIL_BUFFER_BIT);
        auto& transform = registry.get<TransformComponent>(entity);
        auto& mesh = registry.get<BoxMeshComponent>(entity);
        auto& material = registry.get<MaterialComponent>(entity);

        cubeShader->Use();

        cubeShader->SetVec3("material.ambient", material.ambient);
        cubeShader->SetVec3("material.diffuse", material.diffuse);
        cubeShader->SetVec3("material.specular", material.specular);
        cubeShader->SetFloat("material.shininess", material.shininess);

        cubeShader->SetMat4("model", transform.GetModelMatrix());

        glBindVertexArray(mesh.VAO);
        glDrawArrays(GL_TRIANGLES, 0, mesh.numOfVertices);
        glBindVertexArray(0);

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);

        outlineShader->Use();
        outlineShader->SetMat4("model", transform.GetModelMatrix());

        glBindVertexArray(mesh.VAO);
        glDrawArrays(GL_TRIANGLES, 0, mesh.numOfVertices);
        glBindVertexArray(0);
    }
}

void RenderSystem::RenderFloor(entt::registry& registry, Shader* shader) {
    shader->Use();

    auto view = registry.view<TransformComponent, BoxMeshComponent, MaterialComponent>(entt::exclude<OutlineComponent>);
    auto entity  = view.front();
    auto& transform = view.get<TransformComponent>(entity);
    auto& mesh = view.get<BoxMeshComponent>(entity);
    auto& material = view.get<MaterialComponent>(entity);

    shader->SetVec3("material.ambient", material.ambient);
    shader->SetVec3("material.diffuse", material.diffuse);
    shader->SetVec3("material.specular", material.specular);
    shader->SetFloat("material.shininess", material.shininess);

    glBindVertexArray(mesh.VAO);

    shader->SetMat4("model", transform.GetModelMatrix());

    glDrawArrays(GL_TRIANGLES, 0, mesh.numOfVertices);
    glBindVertexArray(0);
}


void RenderSystem::RenderCrosshair(entt::registry& registry, Shader* shader) {
    shader->Use();

    auto view = registry.view<CrosshairMeshComponent>();
    auto entity = view.front();
    auto& mesh = view.get<CrosshairMeshComponent>(entity);

    glBindVertexArray(mesh.VAO);
    glDrawArrays(GL_TRIANGLES, 0, mesh.numOfVertices);
    glBindVertexArray(0);
}

void RenderSystem::RenderGrass(entt::registry& registry, Shader* shader) {
    shader->Use();

    auto view = registry.view<TransformComponent, GrassMeshComponent>();
    for (auto& entity : view) {
        auto& transform = view.get<TransformComponent>(entity);
        auto& mesh = view.get<GrassMeshComponent>(entity);

        shader->SetMat4("model", transform.GetModelMatrix());

        glBindVertexArray(mesh.VAO);
        glDrawArrays(GL_TRIANGLES, 0, mesh.numOfVertices);
        glBindVertexArray(0);
    }
}


void RenderSystem::RenderTree(entt::registry& registry, Shader* modelShader, Shader* outlineShader) {
    auto view = registry.view<TransformComponent, ModelMeshComponent, TreeModelComponent, OutlineComponent>();
    for (auto& entity : view) {
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

void RenderSystem::RenderTrunk(entt::registry& registry, Shader* modelShader, Shader* outlineShader) {
    auto view = registry.view<TransformComponent, ModelMeshComponent, TrunkModelComponent>();
    for (auto& entity : view) {
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

void RenderSystem::RenderRock(entt::registry& registry, Shader* modelShader, Shader* outlineShader) {
    auto view = registry.view<TransformComponent, ModelMeshComponent, RockModelComponent>();
    for (auto& entity : view) {
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

void RenderSystem::RenderBush(entt::registry& registry, Shader* modelShader, Shader* outlineShader) {
    auto view = registry.view<TransformComponent, ModelMeshComponent, BushModelComponent>();
    for (auto& entity : view) {
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

void RenderSystem::RenderMushroom(entt::registry& registry, Shader* modelShader, Shader* outlineShader) {
    auto view = registry.view<TransformComponent, ModelMeshComponent, MushroomModelComponent>();
    for (auto& entity : view) {
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

void RenderSystem::RenderPostProcessing(Shader* shader, unsigned int VAO, unsigned int textureAttachment) {
    shader->Use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureAttachment);
	shader->SetInt("screenTexture", 0);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void RenderSystem::RenderSkybox(Shader* shader, unsigned int VAO, unsigned int skyboxTexture) {
    shader->Use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    shader->SetInt("skybox", 0);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void RenderSystem::RenderScene(entt::registry& registry, SceneShaders& sceneShaders, unsigned int skyboxVAO, unsigned int skyboxTexture) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    RenderSystem::RenderTree(registry, sceneShaders.modelShader, sceneShaders.outlineShader);
    RenderSystem::RenderTrunk(registry, sceneShaders.modelShader, sceneShaders.outlineShader);
    RenderSystem::RenderRock(registry, sceneShaders.modelShader, sceneShaders.outlineShader);
    RenderSystem::RenderBush(registry, sceneShaders.modelShader, sceneShaders.outlineShader);
    RenderSystem::RenderMushroom(registry, sceneShaders.modelShader, sceneShaders.outlineShader);
    glDisable(GL_CULL_FACE);
    RenderSystem::RenderGrass(registry, sceneShaders.grassShader);
    RenderSystem::RenderFloor(registry, sceneShaders.cubeShader);
    RenderSystem::RenderBoxes(registry, sceneShaders.cubeShader, sceneShaders.outlineShader);
    glDepthFunc(GL_LEQUAL);
    RenderSystem::RenderSkybox(sceneShaders.skyboxShader, skyboxVAO, skyboxTexture);
    glDepthFunc(GL_LESS);
}