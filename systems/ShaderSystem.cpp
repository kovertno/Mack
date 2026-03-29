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
    shader->SetVec3("dirLight.ambient", 0.1f, 0.1f, 0.2f);
    shader->SetVec3("dirLight.diffuse", 0.16f, 0.16f, 0.32f);
    shader->SetVec3("dirLight.specular", 0.1f, 0.1f, 0.2f);
}

void ShaderSystem::SetSpotLightStaticUniforms(Shader* shader) {
    shader->Use();

    shader->SetVec3("spotLight.ambient", 0.1f, 0.1f, 0.1f);
    shader->SetVec3("spotLight.diffuse", 0.3f, 0.3f, 0.3f);
    shader->SetVec3("spotLight.specular", 0.1f, 0.1f, 0.1f);
    shader->SetFloat("spotLight.cutOff", cos(glm::radians(11.0f)));
    shader->SetFloat("spotLight.outCutOff", cos(glm::radians(17.5f)));

    shader->SetBool("useSpotLight", true);
}

void ShaderSystem::SetSpotLightDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera) {
    shader->Use();

    shader->SetVec3("spotLight.position", camera->Position);
    shader->SetVec3("spotLight.direction", camera->Front);
}

void ShaderSystem::SetCubeShaderStaticUniforms(Shader* shader) {
    shader->Use();

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (static_cast<float>(Game::SCR_WIDTH) / static_cast<float>(Game::SCR_HEIGHT)), 0.1f, 100.0f);
    shader->SetMat4("projection", projection);
}

void ShaderSystem::SetCubeShaderDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera) {
    shader->Use();

    shader->SetMat4("view", camera->GetViewMatrix());

    shader->SetVec3("viewPos", camera->Position);
}

void ShaderSystem::SetCrosshairShaderStaticUniforms(Shader* shader) {
    shader->Use();

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Game::SCR_WIDTH), 0.0f, static_cast<float>(Game::SCR_HEIGHT));
    shader->SetMat4("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(static_cast<float>(Game::SCR_WIDTH) / 2.0f, static_cast<float>(Game::SCR_HEIGHT) / 2.0f, 0.0f));
    model = glm::scale(model, glm::vec3(5.0f));
    shader->SetMat4("model", model);
}

void ShaderSystem::SetGrassShaderStaticUniforms(Shader* shader) {
    shader->Use();

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(Game::SCR_WIDTH) / static_cast<float>(Game::SCR_HEIGHT), 0.1f, 100.0f);
    shader->SetMat4("projection", projection);
}

void ShaderSystem::SetGrassShaderDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera) {
    shader->Use();

    shader->SetMat4("view", camera->GetViewMatrix());
    shader->SetVec3("viewPos", camera->Position);
}


void ShaderSystem::SetModelShaderStaticUniforms(Shader* shader) {
    shader->Use();

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(Game::SCR_WIDTH) / static_cast<float>(Game::SCR_HEIGHT), 0.1f, 100.0f);
    shader->SetMat4("projection", projection);
}

void ShaderSystem::SetModelShaderDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera) {
    shader->Use();

    shader->SetMat4("view", camera->GetViewMatrix());
    shader->SetVec3("viewPos", camera->Position);
}

void ShaderSystem::SetOutlineShaderStaticUniforms(Shader* shader) {

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), static_cast<float>(Game::SCR_WIDTH) / static_cast<float>(Game::SCR_HEIGHT), 0.1f, 100.0f);

    shader->Use();
    shader->SetMat4("projection", projection);
    shader->SetFloat("scale", 0.015f);
}

void ShaderSystem::SetOutlineShaderDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera) {
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
    ShaderSystem::SetDirectionalLightUniforms(sceneShaders.grassShader);
    ShaderSystem::SetSpotLightStaticUniforms(sceneShaders.cubeShader);
    ShaderSystem::SetSpotLightStaticUniforms(sceneShaders.grassShader);
    ShaderSystem::SetSpotLightStaticUniforms(sceneShaders.modelShader);
    ShaderSystem::SetCrosshairShaderStaticUniforms(sceneShaders.crosshairShader);
    ShaderSystem::SetCubeShaderStaticUniforms(sceneShaders.cubeShader);
    ShaderSystem::SetGrassShaderStaticUniforms(sceneShaders.grassShader);
    ShaderSystem::SetModelShaderStaticUniforms(sceneShaders.modelShader);
    ShaderSystem::SetOutlineShaderStaticUniforms(sceneShaders.outlineShader);
    ShaderSystem::SetPostProcessing(sceneShaders.postProcessingShader, framebufferVAO, framebufferVBO);
    ShaderSystem::SetSkyboxVAO(skyboxVAO, skyboxVBO);
    ShaderSystem::SetSkyboxStaticUnifoms(sceneShaders.skyboxShader);
}

void ShaderSystem::SetDynamicUniforms(SceneShaders& sceneShaders, std::unique_ptr<Camera>& camera) {
    ShaderSystem::SetSpotLightDynamicUniforms(sceneShaders.cubeShader, camera);
    ShaderSystem::SetSpotLightDynamicUniforms(sceneShaders.grassShader, camera);
    ShaderSystem::SetSpotLightDynamicUniforms(sceneShaders.modelShader, camera);
    ShaderSystem::SetCubeShaderDynamicUniforms(sceneShaders.cubeShader, camera);
    ShaderSystem::SetGrassShaderDynamicUniforms(sceneShaders.grassShader, camera);
    ShaderSystem::SetModelShaderDynamicUniforms(sceneShaders.modelShader, camera);
    ShaderSystem::SetOutlineShaderDynamicUniforms(sceneShaders.outlineShader, camera);
    ShaderSystem::SetSkyboxDynamicUniforms(sceneShaders.skyboxShader, camera);
}