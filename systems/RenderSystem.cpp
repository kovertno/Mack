#include "Shader.hpp"
#include "Camera.hpp"
#include "Game.hpp"

#include "RenderSystem.h"
#include "TransformComponent.hpp"
#include "BoxMeshComponent.hpp"

#include <entt/entt.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void RenderSystem::SetBoxStaticUniforms(Shader* shader, std::unique_ptr<Camera>& camera) {
    shader->Use();

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)Game::SCR_WIDTH / (float)Game::SCR_HEIGHT, 0.1f, 100.0f);
    shader->SetMat4("projection", projection);

    shader->SetVec3("material.ambient", 0.3f, 0.5f, 0.0f);
    shader->SetVec3("material.diffuse", 0.6f, 1.0f, 0.0f);
    shader->SetVec3("material.specular", 1.0f, 1.0f, 0.0f);
    shader->SetFloat("material.shininess", 32.0f);

    shader->SetVec3("dirLight.direction", 0.0f, -2.0f, -3.0f);
    shader->SetVec3("dirLight.ambient", 0.3f, 0.3f, 0.3f);
    shader->SetVec3("dirLight.diffuse", 0.7f, 0.7f, 0.7f);
    shader->SetVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
}

void RenderSystem::SetBoxDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera) {
    shader->Use();

    shader->SetMat4("view", camera->GetViewMatrix());

    shader->SetVec3("viewPos", camera->Position);
}

void RenderSystem::RenderBoxes(entt::registry& registry, Shader* shader) {
    shader->Use();

    auto view = registry.view<TransformComponent, BoxMeshComponent>();
    for (auto entity : view) {
        auto& mesh = view.get<BoxMeshComponent>(entity);
        auto& transform = view.get<TransformComponent>(entity);

        glBindVertexArray(mesh.VAO);

        shader->SetMat4("model", transform.GetModelMatrix());

        glDrawArrays(GL_TRIANGLES, 0, mesh.numOfVertices);
    }
}