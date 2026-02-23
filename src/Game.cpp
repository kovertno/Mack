#include "Game.hpp"
#include "Window.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

#include "TransformComponent.hpp"
#include "BoxMeshComponent.hpp"
#include "CrosshairMeshComponent.hpp"
#include "MaterialComponent.hpp"

#include "RenderSystem.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <entt/entt.hpp>

#include <iostream>

void Game::Init() {
    Window _window{SCR_WIDTH, SCR_HEIGHT};
	window = _window.InitWindow();

    SetCallbacks();
    
    camera = std::make_unique<Camera>();  

    crosshairShader = std::make_unique<Shader>("resources/shaders/crosshairShader.vs", "resources/shaders/crosshairShader.fs");
    cubeShader = std::make_unique<Shader>("resources/shaders/cubeShader.vs", "resources/shaders/cubeShader.fs");

    CrosshairMeshComponent::SetCrosshairVAO(crosshairVAO, crosshairVAO);
    BoxMeshComponent::SetBoxVAO(cubeVAO, cubeVBO);

    entt::entity crosshairEntity = registry.create();
    auto& crosshairMesh = registry.emplace<CrosshairMeshComponent>(crosshairEntity);
    crosshairMesh.VAO = crosshairVAO;
    crosshairMesh.numOfVertices = 6;

    entt::entity cube1Entity = registry.create();
    auto& box1Transform = registry.emplace<TransformComponent>(cube1Entity);
    box1Transform.position = glm::vec3{ 0.0f, 0.25f, -2.0f };
    box1Transform.scale.y = 1.5f;
    auto& cube1Material = registry.emplace<MaterialComponent>(cube1Entity);
    cube1Material.ambient = glm::vec3(0.3f, 0.0f, 0.0f);
    cube1Material.diffuse = glm::vec3(0.7f, 0.0f, 0.0f);
    cube1Material.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    auto& box1Mesh = registry.emplace<BoxMeshComponent>(cube1Entity);
    box1Mesh.VAO = cubeVAO;
    box1Mesh.numOfVertices = 36;

    entt::entity cube2Entity = registry.create();
    auto& box2Transform = registry.emplace<TransformComponent>(cube2Entity);
    box2Transform.position = glm::vec3{ 3.0f, 0.25f, -8.0f };
    box2Transform.scale.y = 1.5f;
    auto& cube2Material = registry.emplace<MaterialComponent>(cube2Entity);
    cube2Material.ambient = glm::vec3(0.3f, 0.0f, 0.0f);
    cube2Material.diffuse = glm::vec3(0.7f, 0.0f, 0.0f);
    cube2Material.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    auto& box2Mesh = registry.emplace<BoxMeshComponent>(cube2Entity);
    box2Mesh.VAO = cubeVAO;
    box2Mesh.numOfVertices = 36;

    entt::entity cube3Entity = registry.create();
    auto& box3Transform = registry.emplace<TransformComponent>(cube3Entity);
    box3Transform.position = glm::vec3{ -6.0f, 0.25f, -10.0f };
    box3Transform.scale.y = 1.5f;
    auto& cube3Material = registry.emplace<MaterialComponent>(cube3Entity);
    cube3Material.ambient = glm::vec3(0.3f, 0.0f, 0.0f);
    cube3Material.diffuse = glm::vec3(0.7f, 0.0f, 0.0f);
    cube3Material.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    auto& box3Mesh = registry.emplace<BoxMeshComponent>(cube3Entity);
    box3Mesh.VAO = cubeVAO;
    box3Mesh.numOfVertices = 36;

    entt::entity floorEntity = registry.create();
    auto& floorTransform = registry.emplace<TransformComponent>(floorEntity);
    floorTransform.position = glm::vec3(0.0f, -0.6f, 0.0f);
    floorTransform.rotation = glm::vec3(90.0f, 0.0f, 0.0f);
    floorTransform.scale = glm::vec3(30.0f, 30.0f, 0.2f);
    auto& floorMaterial = registry.emplace<MaterialComponent>(floorEntity);
    floorMaterial.ambient = glm::vec3(0.0f, 0.3f, 0.0f);
    floorMaterial.diffuse = glm::vec3(0.0f, 0.7f, 0.0f);
    floorMaterial.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    auto& floorMesh = registry.emplace<BoxMeshComponent>(floorEntity);
    floorMesh.VAO = cubeVAO;
    floorMesh.numOfVertices = 36;

    RenderSystem::SetDirectionalLightUniforms(cubeShader.get());
    RenderSystem::SetCrosshairStaticUniforms(crosshairShader.get());
    RenderSystem::SetBoxStaticUniforms(cubeShader.get());
}

void Game::Run() {
      while (!glfwWindowShouldClose(window)) {
          // clear buffers
          glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

          ProcessInput();
          Update();
          Render();

          glfwSwapBuffers(window);
          glfwPollEvents();
      }
}

void Game::ProcessInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->KeyboardMovement(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->KeyboardMovement(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->KeyboardMovement(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->KeyboardMovement(RIGHT, deltaTime);
}

void Game::Update() {
    float currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void Game::Render() {
    RenderSystem::SetBoxDynamicUniforms(cubeShader.get(), camera);

    // enable depth testing for 3d world
    glEnable(GL_DEPTH_TEST);
    RenderSystem::RenderBoxes(registry, cubeShader.get());

    // disable depth testing for 2d elements
    glDisable(GL_DEPTH_TEST);
    RenderSystem::RenderCrosshair(registry, crosshairShader.get());
}

void Game::SetCallbacks() {
    // we set pointer for the window to this class instance
    glfwSetWindowUserPointer(window, this);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
}

void Game::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Game::mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn) {
    // we get the pointer set to this class instance in funciton SetCallbacks()
    Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));

    float xpos = static_cast<float>(xPosIn);
    float ypos = static_cast<float>(yPosIn);

    if (game->isFirstMouse) {
        game->lastX = xpos;
        game->lastY = ypos;
        game->isFirstMouse = false;
    }

    float xoffset = xpos - game->lastX;
    float yoffset = game->lastY - ypos;

    game->lastX = xpos;
    game->lastY = ypos;

    game->camera->MouseMovement(xoffset, yoffset);
}