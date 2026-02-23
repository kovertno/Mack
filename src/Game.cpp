#include "Game.hpp"
#include "Window.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

#include "TransformComponent.hpp"
#include "BoxMeshComponent.hpp"
#include "CrosshairMeshComponent.hpp"

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
    cubeShader = std::make_unique<Shader>("resources/shaders/cubeShader.vs", "resources/shaders/cubeShader.fs");
    crosshairShader = std::make_unique<Shader>("resources/shaders/crosshairShader.vs", "resources/shaders/crosshairShader.fs");

    RenderSystem::SetBoxStaticUniforms(cubeShader.get());
    RenderSystem::SetCrosshairStaticUniforms(crosshairShader.get());

    BoxMeshComponent::SetBoxVAO(cubeVAO, cubeVBO);
    CrosshairMeshComponent::SetCrosshairVAO(crosshairVAO, crosshairVAO);

    entt::entity cubeEntity = registry.create();
    auto& transform = registry.emplace<TransformComponent>(cubeEntity);
    transform.position = glm::vec3{ 0.0f, 0.0f, -2.0f };
    auto& boxMesh = registry.emplace<BoxMeshComponent>(cubeEntity);
    boxMesh.VAO = cubeVAO;
    boxMesh.numOfVertices = 36;

    entt::entity crosshairEntity = registry.create();
    auto& crosshairMesh = registry.emplace<CrosshairMeshComponent>(crosshairEntity);
    crosshairMesh.VAO = crosshairVAO;
    crosshairMesh.numOfVertices = 6;
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