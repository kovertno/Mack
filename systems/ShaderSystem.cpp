#include "ShaderSystem.h"

#include "Shader.hpp"
#include "Camera.hpp"
#include "Game.hpp"
#include "SceneShaders.hpp"

#include "TransformComponent.hpp"
#include "ModelMeshComponent.hpp"
#include "FireflyModelComponent.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void ShaderSystem::CreateMatricesUBO(unsigned int& matricesUBO) {
    glGenBuffers(1, &matricesUBO);

    glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, matricesUBO, 0, 2 * sizeof(glm::mat4) /* 2 * 4 * 16 = 128 */);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ShaderSystem::UpdateMatricesUBO(unsigned int& matricesUBO, std::unique_ptr<Camera>& camera) {
    static glm::mat4 projection = glm::perspective(glm::radians(45.0f), (static_cast<float>(Game::SCR_WIDTH) / static_cast<float>(Game::SCR_HEIGHT)), 0.1f, 100.0f);
    glm::mat4 view = camera->GetViewMatrix();

    glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ShaderSystem::CreateDataUBO(unsigned int& dataUBO) {
    glGenBuffers(1, &dataUBO);

    glBindBuffer(GL_UNIFORM_BUFFER, dataUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), NULL, GL_STATIC_DRAW);

    glBindBufferRange(GL_UNIFORM_BUFFER, 1, dataUBO, 0, sizeof(glm::vec3) /* 16 */);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void ShaderSystem::UpdateDataUBO(unsigned int& dataUBO, std::unique_ptr<Camera>& camera) {
    glm::vec3 viewPos = camera->Position;

    glBindBuffer(GL_UNIFORM_BUFFER, dataUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(viewPos));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

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
}

void ShaderSystem::SetSpotLightDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera, bool useSpotLight) {
    shader->Use();

    shader->SetVec3("spotLight.position", camera->Position);
    shader->SetVec3("spotLight.direction", camera->Front);

    if (useSpotLight)
        shader->SetBool("useSpotLight", true);
    else
        shader->SetBool("useSpotLight", false);
}

void ShaderSystem::SetPointLightStaticUniforms(Shader* shader) {
    shader->Use();

    for (uint16_t i = 0; i < 5; ++i) { // there are 5 hardcoded fireflies for now
        shader->SetFloat("pointLight[" + std::to_string(i) + "].constant", 1.0f);
        shader->SetFloat("pointLight[" + std::to_string(i) + "].linear", 0.35f);
        shader->SetFloat("pointLight[" + std::to_string(i) + "].quadratic", 0.44f);
        shader->SetVec3("pointLight[" + std::to_string(i) + "].ambient", glm::vec3(0.3f, 0.15f, 0.15f));
        shader->SetVec3("pointLight[" + std::to_string(i) + "].diffuse", glm::vec3(0.5f, 0.35f, 0.35f));
        shader->SetVec3("pointLight[" + std::to_string(i) + "].specular", glm::vec3(0.5f, 0.35f, 0.35f));

        shader->SetBool("usePointLight", true);
    }
}

void ShaderSystem::SetPointLightDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera, entt::registry& registry) {
    shader->Use();

    uint16_t i = 0;
    auto view = registry.view<TransformComponent, ModelMeshComponent, FireflyModelComponent>();
    for (auto entity : view) {
        auto transform = registry.get<TransformComponent>(entity);
        shader->SetVec3("pointLight[" + std::to_string(i) + "].position", transform.position);
        ++i;
    }
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

void ShaderSystem::SetOutlineShaderStaticUniforms(Shader* shader) {
    shader->Use();
    shader->SetFloat("scale", 0.015f);
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

void ShaderSystem::SetStaticUniforms(SceneShaders& sceneShaders, unsigned int& framebufferVAO, unsigned int& framebufferVBO, unsigned int& skyboxVAO, unsigned int& skyboxVBO) {
    ShaderSystem::SetDirectionalLightUniforms(sceneShaders.cubeShader);
    ShaderSystem::SetDirectionalLightUniforms(sceneShaders.modelShader);
    ShaderSystem::SetDirectionalLightUniforms(sceneShaders.grassShader);
    ShaderSystem::SetSpotLightStaticUniforms(sceneShaders.cubeShader);
    ShaderSystem::SetSpotLightStaticUniforms(sceneShaders.grassShader);
    ShaderSystem::SetSpotLightStaticUniforms(sceneShaders.modelShader);
    ShaderSystem::SetPointLightStaticUniforms(sceneShaders.cubeShader);
    ShaderSystem::SetPointLightStaticUniforms(sceneShaders.grassShader);
    ShaderSystem::SetPointLightStaticUniforms(sceneShaders.modelShader);
    ShaderSystem::SetCrosshairShaderStaticUniforms(sceneShaders.crosshairShader);
    ShaderSystem::SetOutlineShaderStaticUniforms(sceneShaders.outlineShader);
    ShaderSystem::SetPostProcessing(sceneShaders.postProcessingShader, framebufferVAO, framebufferVBO);
    ShaderSystem::SetSkyboxVAO(skyboxVAO, skyboxVBO);
}

void ShaderSystem::SetDynamicUniforms(SceneShaders& sceneShaders, std::unique_ptr<Camera>& camera, entt::registry& registry, bool useSpotLight, unsigned int& matricesUBO, unsigned int& dataUBO) {
    ShaderSystem::UpdateMatricesUBO(matricesUBO, camera);
    ShaderSystem::UpdateDataUBO(dataUBO, camera);
    ShaderSystem::SetSpotLightDynamicUniforms(sceneShaders.cubeShader, camera, useSpotLight);
    ShaderSystem::SetSpotLightDynamicUniforms(sceneShaders.grassShader, camera, useSpotLight);
    ShaderSystem::SetSpotLightDynamicUniforms(sceneShaders.modelShader, camera, useSpotLight);
    ShaderSystem::SetPointLightDynamicUniforms(sceneShaders.cubeShader, camera, registry);
    ShaderSystem::SetPointLightDynamicUniforms(sceneShaders.grassShader, camera, registry);
    ShaderSystem::SetPointLightDynamicUniforms(sceneShaders.modelShader, camera, registry);
}