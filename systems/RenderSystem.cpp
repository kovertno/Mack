#include "Shader.hpp"
#include "Camera.hpp"
#include "Game.hpp"

#include "RenderSystem.h"
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

void RenderSystem::SetDirectionalLightUniforms(Shader* shader) {
    shader->Use();

    shader->SetVec3("dirLight.direction", 0.0f, -2.0f, -3.0f);
    shader->SetVec3("dirLight.ambient", 0.3f, 0.3f, 0.3f);
    shader->SetVec3("dirLight.diffuse", 0.7f, 0.7f, 0.7f);
    shader->SetVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
}

void RenderSystem::SetBoxStaticUniforms(Shader* shader) {
    shader->Use();

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)Game::SCR_WIDTH / (float)Game::SCR_HEIGHT, 0.1f, 100.0f);
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

    glm::mat4 projection = glm::ortho(0.0f, (float)Game::SCR_WIDTH, 0.0f, (float)Game::SCR_HEIGHT, -1.0f, 1.0f);
    shader->SetMat4("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3((float)Game::SCR_WIDTH/2.0f, (float)Game::SCR_HEIGHT/2.0f, 0.0f));
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
    projection = glm::perspective(glm::radians(45.0f), (float)Game::SCR_WIDTH / (float)Game::SCR_HEIGHT, 0.1f, 100.0f);
    shader->SetMat4("projection", projection);
}

void RenderSystem::SetGrassDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera) {
    shader->Use();

    shader->SetMat4("view", camera->GetViewMatrix());
}

void RenderSystem::RenderGrass(entt::registry& registry, Shader* shader) {
    shader->Use();

    auto view = registry.view<TransformComponent, GrassMeshComponent>();
    for (auto entity : view) {
        auto& transform = view.get<TransformComponent>(entity);
        auto& mesh = view.get<GrassMeshComponent>(entity);

        shader->SetMat4("model", transform.GetModelMatrix());

        glBindVertexArray(mesh.VAO);
        glDrawArrays(GL_TRIANGLES, 0, mesh.numOfVertices);
    }
}