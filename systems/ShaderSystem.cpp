#include "ShaderSystem.h"

#include "Shader.hpp"
#include "Camera.hpp"
#include "Game.hpp"
#include "SceneShaders.hpp"

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

void ShaderSystem::SetPostProcessing(Shader* shader, unsigned int& VAO, unsigned int& VBO) {
    float quadVertices[] = { // fills the entire screen in NDCs.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

	glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void ShaderSystem::SetSkyboxVAO(unsigned int& VAO, unsigned int& VBO) {
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void ShaderSystem::SetSkyboxStaticUnifoms(Shader* shader) {
    shader->Use();
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(Game::SCR_WIDTH) / static_cast<float>(Game::SCR_HEIGHT), 0.1f, 100.0f);
    shader->SetMat4("projection", projection);
}

void ShaderSystem::SetSkyboxDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera) {
    shader->Use();
    glm::mat4 view = glm::mat4(glm::mat3(camera->GetViewMatrix()));
    shader->SetMat4("view", view);
}

void ShaderSystem::SetStaticUniforms(SceneShaders& sceneShaders, unsigned int& framebufferVAO, unsigned int& framebufferVBO, unsigned int& skyboxVAO, unsigned int& skyboxVBO) {
    ShaderSystem::SetDirectionalLightUniforms(sceneShaders.cubeShader);
    ShaderSystem::SetDirectionalLightUniforms(sceneShaders.modelShader);
    ShaderSystem::SetCrosshairStaticUniforms(sceneShaders.crosshairShader);
    ShaderSystem::SetBoxStaticUniforms(sceneShaders.cubeShader);
    ShaderSystem::SetGrassStaticUniforms(sceneShaders.grassShader);
    ShaderSystem::SetTreeStaticUniforms(sceneShaders.modelShader);
    ShaderSystem::SetTrunkStaticUniforms(sceneShaders.modelShader);
    ShaderSystem::SetRockStaticUniforms(sceneShaders.modelShader);
    ShaderSystem::SetBushStaticUniforms(sceneShaders.modelShader);
    ShaderSystem::SetMushroomStaticUniforms(sceneShaders.modelShader);
    ShaderSystem::SetOutlineStaticUniforms(sceneShaders.outlineShader);
    ShaderSystem::SetPostProcessing(sceneShaders.postProcessingShader, framebufferVAO, framebufferVBO);
    ShaderSystem::SetSkyboxVAO(skyboxVAO, skyboxVBO);
    ShaderSystem::SetSkyboxStaticUnifoms(sceneShaders.skyboxShader);
}

void ShaderSystem::SetDynamicUniforms(SceneShaders& sceneShaders, std::unique_ptr<Camera>& camera) {
    ShaderSystem::SetBoxDynamicUniforms(sceneShaders.cubeShader, camera);
    ShaderSystem::SetGrassDynamicUniforms(sceneShaders.grassShader, camera);
    ShaderSystem::SetTreeDynamicUniforms(sceneShaders.modelShader, camera);
    ShaderSystem::SetTrunkDynamicUniforms(sceneShaders.modelShader, camera);
    ShaderSystem::SetRockDynamicUniforms(sceneShaders.modelShader, camera);
    ShaderSystem::SetBushDynamicUniforms(sceneShaders.modelShader, camera);
    ShaderSystem::SetMushroomDynamicUniforms(sceneShaders.modelShader, camera);
    ShaderSystem::SetOutlineDynamicUniforms(sceneShaders.outlineShader, camera);
    ShaderSystem::SetSkyboxDynamicUniforms(sceneShaders.skyboxShader, camera);
}