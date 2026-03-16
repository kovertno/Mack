#include "Shader.hpp"
#include "Camera.hpp"
#include "Game.hpp"

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

#include <entt/entt.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void RenderSystem::SetDirectionalLightUniforms(Shader* shader) {
    shader->Use();

    shader->SetVec3("dirLight.direction", 0.0f, -2.0f, -3.0f);
    shader->SetVec3("dirLight.ambient", 0.85f, 0.85f, 0.85f);
    shader->SetVec3("dirLight.diffuse", 0.65f, 0.65f, 0.65f);
    shader->SetVec3("dirLight.specular", 0.4f, 0.4f, 0.4f);
}

void RenderSystem::SetBoxStaticUniforms(Shader* shader) {
    shader->Use();

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(Game::SCR_WIDTH) / static_cast<float>(Game::SCR_HEIGHT), 0.1f, 100.0f);
    shader->SetMat4("projection", projection);
}

void RenderSystem::SetBoxDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera) {
    shader->Use();

    shader->SetMat4("view", camera->GetViewMatrix());

    shader->SetVec3("viewPos", camera->Position);
}

void RenderSystem::RenderBoxes(entt::registry& registry, Shader* shader) {
    shader->Use();

    auto view = registry.view<TransformComponent, BoxMeshComponent, MaterialComponent>();
    for (auto entity : view) {
        auto& mesh = view.get<BoxMeshComponent>(entity);
        auto& transform = view.get<TransformComponent>(entity);
        auto& material = view.get<MaterialComponent>(entity);

        shader->SetVec3("material.ambient", material.ambient);
        shader->SetVec3("material.diffuse", material.diffuse);
        shader->SetVec3("material.specular", material.specular);
        shader->SetFloat("material.shininess", material.shininess);

        glBindVertexArray(mesh.VAO);

        shader->SetMat4("model", transform.GetModelMatrix());

        glDrawArrays(GL_TRIANGLES, 0, mesh.numOfVertices);
    }
}

void RenderSystem::SetCrosshairStaticUniforms(Shader* shader) {
    shader->Use();

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Game::SCR_WIDTH) / static_cast<float>(Game::SCR_HEIGHT), -1.0f, 1.0f);
    shader->SetMat4("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(static_cast<float>(Game::SCR_WIDTH)/2.0f, static_cast<float>(Game::SCR_HEIGHT)/2.0f, 0.0f));
    model = glm::scale(model, glm::vec3(5.0f));
    shader->SetMat4("model", model);
}

void RenderSystem::RenderCrosshair(entt::registry& registry, Shader* shader) {
    shader->Use();

    auto view = registry.view<CrosshairMeshComponent>();
    auto entity = view.front();
    auto& mesh = view.get<CrosshairMeshComponent>(entity);

    glBindVertexArray(mesh.VAO);
    glDrawArrays(GL_TRIANGLES, 0, mesh.numOfVertices);
}

void RenderSystem::SetGrassStaticUniforms(Shader* shader) {
    shader->Use();

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(Game::SCR_WIDTH) / static_cast<float>(Game::SCR_HEIGHT), 0.1f, 100.0f);
    shader->SetMat4("projection", projection);
}

void RenderSystem::SetGrassDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera) {
    shader->Use();

    shader->SetMat4("view", camera->GetViewMatrix());
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
    }
}

void RenderSystem::SetBackpackStaticUniforms(Shader* shader) {
    shader->Use();

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(Game::SCR_WIDTH) / static_cast<float>(Game::SCR_HEIGHT), 0.1f, 100.0f);
    shader->SetMat4("projection", projection);
}

void RenderSystem::SetBackpackDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera) {
    shader->Use();

    shader->SetMat4("view", camera->GetViewMatrix());

    shader->SetVec3("viewPos", camera->Position);
}
 
void RenderSystem::RenderBackpack(entt::registry& registry, Shader* shader) {
    shader->Use();

    auto view = registry.view<TransformComponent, ModelMeshComponent>();
    auto entity = view.front();
    auto& transform = view.get<TransformComponent>(entity);
    auto& mesh = view.get<ModelMeshComponent>(entity);

    shader->SetMat4("model", transform.GetModelMatrix());
    shader->SetFloat("material.shininess", 32.0f);

    mesh.model.Draw(shader);
} 

void RenderSystem::SetTreeStaticUniforms(Shader* shader) {
    shader->Use();

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(Game::SCR_WIDTH)/static_cast<float>(Game::SCR_HEIGHT), 0.1f, 100.0f);
    shader->SetMat4("projection", projection);
}

void RenderSystem::SetTreeDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera) {
    shader->Use();

    shader->SetMat4("view", camera->GetViewMatrix());
    shader->SetVec3("viewPos", camera->Position);
}

void RenderSystem::RenderTree(entt::registry& registry, Shader* shader) {
    shader->Use();

    auto view = registry.view<TransformComponent, ModelMeshComponent, TreeModelComponent>();
    for (auto& entity : view) {
        auto& transform = view.get<TransformComponent>(entity);
        auto& mesh = view.get<ModelMeshComponent>(entity);

        shader->SetMat4("model", transform.GetModelMatrix());
        shader->SetFloat("material.shininess", 32.0f);
        
        mesh.model.Draw(shader);
    }
}

void RenderSystem::SetTrunkStaticUniforms(Shader* shader) {
    shader->Use();

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(Game::SCR_WIDTH) / static_cast<float>(Game::SCR_HEIGHT), 0.1f, 100.0f);
    shader->SetMat4("projection", projection);
}

void RenderSystem::SetTrunkDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera) {
    shader->Use();

    shader->SetVec3("viewPos", camera->Position);
    shader->SetMat4("view", camera->GetViewMatrix());
}

void RenderSystem::RenderTrunk(entt::registry& registry, Shader* shader) {
    shader->Use();

    auto view = registry.view<TransformComponent, ModelMeshComponent, TrunkModelComponent>();
    for (auto& entity : view) {
        auto& transform = view.get<TransformComponent>(entity);
        auto& mesh = view.get<ModelMeshComponent>(entity);

        shader->SetMat4("model", transform.GetModelMatrix());
        shader->SetFloat("material.shininess", 32.0f);

        mesh.model.Draw(shader);
    }
}

void RenderSystem::SetRockStaticUniforms(Shader* shader) {
    shader->Use();

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(Game::SCR_WIDTH)/static_cast<float>(Game::SCR_HEIGHT), 0.1f, 100.0f);
    shader->SetMat4("projection", projection);
}

void RenderSystem::SetRockDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera) {
    shader->Use();

    shader->SetVec3("viewPos", camera->Position);
    shader->SetMat4("view", camera->GetViewMatrix());
}

void RenderSystem::RenderRock(entt::registry& registry, Shader* shader) {
    shader->Use();

    auto view = registry.view<TransformComponent, ModelMeshComponent, RockModelComponent>();
    for (auto& entity : view) {
        auto& transform = view.get<TransformComponent>(entity);
        auto& mesh = view.get<ModelMeshComponent>(entity);

        shader->SetMat4("model", transform.GetModelMatrix());
        shader->SetFloat("material.shininess", 32.0f);

        mesh.model.Draw(shader);
    }
}

void RenderSystem::SetBushStaticUniforms(Shader* shader) {
    shader->Use();

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(Game::SCR_WIDTH) / static_cast<float>(Game::SCR_HEIGHT), 0.1f, 100.0f);
    shader->SetMat4("projection", projection);
}

void RenderSystem::SetBushDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera) {
    shader->Use();

    shader->SetVec3("viewPos", camera->Position);
    shader->SetMat4("view", camera->GetViewMatrix());
}

void RenderSystem::RenderBush(entt::registry& registry, Shader* shader) {
    shader->Use();

    auto view = registry.view<TransformComponent, ModelMeshComponent, BushModelComponent>();
    for (auto& entity : view) {
        auto& transform = registry.get<TransformComponent>(entity);
        auto& mesh = registry.get<ModelMeshComponent>(entity);

        shader->SetMat4("model", transform.GetModelMatrix());
        shader->SetFloat("material.shininess", 32.0f);

        mesh.model.Draw(shader);
    }
}

void RenderSystem::SetMushroomStaticUniforms(Shader* shader) {
    shader->Use();

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(Game::SCR_WIDTH) / static_cast<float>(Game::SCR_HEIGHT), 0.1f, 100.0f);
    shader->SetMat4("projection", projection);
}

void RenderSystem::SetMushroomDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera) {
    shader->Use();

    shader->SetVec3("viewPos", camera->Position);
    shader->SetMat4("view", camera->GetViewMatrix());
}

void RenderSystem::RenderMushroom(entt::registry& registry, Shader* shader) {
    shader->Use();

    auto view = registry.view<TransformComponent, ModelMeshComponent, MushroomModelComponent>();
    for (auto& entity : view) {
        auto& transform = registry.get<TransformComponent>(entity);
        auto& mesh = registry.get<ModelMeshComponent>(entity);

        shader->SetMat4("model", transform.GetModelMatrix());
        shader->SetFloat("material.shininess", 32.0f);

        mesh.model.Draw(shader);
    }
}