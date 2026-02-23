#include "Shader.hpp"
#include "Camera.hpp"
#include "Game.hpp"

#include "RenderSystem.h"
#include "TransformComponent.hpp"
#include "BoxMeshComponent.hpp"

#include <entt/entt.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void RenderSystem::Render(entt::registry& registry, Shader* shader, std::unique_ptr<Camera>& camera) {
    shader->Use();

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)Game::SCR_WIDTH / (float)Game::SCR_HEIGHT, 0.1f, 100.0f);
    shader->SetMat4("projection", projection);

    shader->SetMat4("view", camera->GetViewMatrix());

    auto view = registry.view<TransformComponent, BoxMeshComponent>();
    for (auto entity : view) {
        auto& mesh = view.get<BoxMeshComponent>(entity);
        auto& transform = view.get<TransformComponent>(entity);

        glBindVertexArray(mesh.GetVAO());

        shader->SetMat4("model", transform.GetModelMatrix());

        glDrawArrays(GL_TRIANGLES, 0, mesh.numOfVertices);
    }
}