#include "ShaderSystem.h"

#include "Shader.hpp"
#include "Camera.hpp"
#include "Game.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void ShaderSystem::SetDirectionalLightUniforms(Shader* shader) {
    shader->Use();

    shader->SetVec3("dirLight.direction", 0.0f, -2.0f, -3.0f);
    shader->SetVec3("dirLight.ambient", 0.85f, 0.85f, 0.85f);
    shader->SetVec3("dirLight.diffuse", 0.65f, 0.65f, 0.65f);
    shader->SetVec3("dirLight.specular", 0.4f, 0.4f, 0.4f);
}

void ShaderSystem::SetBoxStaticUniforms(Shader* shader) {
    shader->Use();

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (static_cast<float>(Game::SCR_WIDTH) / static_cast<float>(Game::SCR_HEIGHT)), 0.1f, 100.0f);
    shader->SetMat4("projection", projection);
}

void ShaderSystem::SetBoxDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera) {
    shader->Use();

    shader->SetMat4("view", camera->GetViewMatrix());

    shader->SetVec3("viewPos", camera->Position);
}

void ShaderSystem::SetCrosshairStaticUniforms(Shader* shader) {
    shader->Use();

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Game::SCR_WIDTH), 0.0f, static_cast<float>(Game::SCR_HEIGHT));
    shader->SetMat4("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(static_cast<float>(Game::SCR_WIDTH) / 2.0f, static_cast<float>(Game::SCR_HEIGHT) / 2.0f, 0.0f));
    model = glm::scale(model, glm::vec3(5.0f));
    shader->SetMat4("model", model);
}

void ShaderSystem::SetGrassStaticUniforms(Shader* shader) {
    shader->Use();

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(Game::SCR_WIDTH) / static_cast<float>(Game::SCR_HEIGHT), 0.1f, 100.0f);
    shader->SetMat4("projection", projection);
}

void ShaderSystem::SetGrassDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera) {
    shader->Use();

    shader->SetMat4("view", camera->GetViewMatrix());
}


void ShaderSystem::SetTreeStaticUniforms(Shader* shader) {
    shader->Use();

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(Game::SCR_WIDTH) / static_cast<float>(Game::SCR_HEIGHT), 0.1f, 100.0f);
    shader->SetMat4("projection", projection);
}

void ShaderSystem::SetTreeDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera) {
    shader->Use();

    shader->SetMat4("view", camera->GetViewMatrix());
    shader->SetVec3("viewPos", camera->Position);
}


void ShaderSystem::SetTrunkStaticUniforms(Shader* shader) {
    shader->Use();

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(Game::SCR_WIDTH) / static_cast<float>(Game::SCR_HEIGHT), 0.1f, 100.0f);
    shader->SetMat4("projection", projection);
}

void ShaderSystem::SetTrunkDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera) {
    shader->Use();

    shader->SetVec3("viewPos", camera->Position);
    shader->SetMat4("view", camera->GetViewMatrix());
}

void ShaderSystem::SetRockStaticUniforms(Shader* shader) {
    shader->Use();

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(Game::SCR_WIDTH) / static_cast<float>(Game::SCR_HEIGHT), 0.1f, 100.0f);
    shader->SetMat4("projection", projection);
}

void ShaderSystem::SetRockDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera) {
    shader->Use();

    shader->SetVec3("viewPos", camera->Position);
    shader->SetMat4("view", camera->GetViewMatrix());
}

void ShaderSystem::SetBushStaticUniforms(Shader* shader) {
    shader->Use();

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(Game::SCR_WIDTH) / static_cast<float>(Game::SCR_HEIGHT), 0.1f, 100.0f);
    shader->SetMat4("projection", projection);
}

void ShaderSystem::SetBushDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera) {
    shader->Use();

    shader->SetVec3("viewPos", camera->Position);
    shader->SetMat4("view", camera->GetViewMatrix());
}

void ShaderSystem::SetMushroomStaticUniforms(Shader* shader) {
    shader->Use();

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(Game::SCR_WIDTH) / static_cast<float>(Game::SCR_HEIGHT), 0.1f, 100.0f);
    shader->SetMat4("projection", projection);
}

void ShaderSystem::SetMushroomDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera) {
    shader->Use();

    shader->SetVec3("viewPos", camera->Position);
    shader->SetMat4("view", camera->GetViewMatrix());
}

void ShaderSystem::SetOutlineStaticUniforms(Shader* shader) {

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(Game::SCR_WIDTH) / static_cast<float>(Game::SCR_HEIGHT), 0.1f, 100.0f);

    shader->Use();
    shader->SetMat4("projection", projection);
    shader->SetFloat("scale", 0.015f);
}

void ShaderSystem::SetOutlineDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera) {
    shader->Use();
    shader->SetMat4("view", camera->GetViewMatrix());
}