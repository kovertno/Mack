#include "Game.hpp"
#include "Window.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "EntityManager.hpp"
#include "ModelMesh.hpp"
#include "Model.hpp"

#include "RenderSystem.h"
#include "PhysicsSystem.h"
#include "CollisionSystem.hpp"
#include "RayCastSystem.hpp"
#include "KnockBackSystem.hpp"
#include "ShaderSystem.h"

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

    entityManager = std::make_unique<EntityManager>(registry);
    
    camera = std::make_unique<Camera>();  

    crosshairShader = std::make_unique<Shader>("resources/shaders/crosshairShader.vs", "resources/shaders/crosshairShader.fs");
    cubeShader = std::make_unique<Shader>("resources/shaders/cubeShader.vs", "resources/shaders/cubeShader.fs");
    grassShader = std::make_unique<Shader>("resources/shaders/grassShader.vs", "resources/shaders/grassShader.fs");
    modelShader = std::make_unique<Shader>("resources/shaders/modelShader.vs", "resources/shaders/modelShader.fs");
    outlineShader = std::make_unique<Shader>("resources/shaders/outlineShader.vs", "resources/shaders/outlineShader.fs");

    CrosshairMeshComponent::SetCrosshairVAO(crosshairVAO, crosshairVBO);
    BoxMeshComponent::SetBoxVAO(cubeVAO, cubeVBO);
    GrassMeshComponent::SetGrassVAO(grassVAO, grassVBO);
    // crosshair
    entityManager->CreateCrosshair(crosshairVAO);
    // enemies
    unsigned int numOfEnemies = 3;
    entityManager->CreateEnemies(cubeVAO, numOfEnemies);
    // floor
    entityManager->CreateFloor(cubeVAO);
    // grass
    entityManager->CreateGrass(grassVAO);
    // tree
    entityManager->CreateTreeModel();
    // trunk
    entityManager->CreateTrunkModel();
    // rock
    entityManager->CreateRockModel();
    // bush
    entityManager->CreateBushModel();
    // mushroom
    entityManager->CreateMushroomModel();

    ShaderSystem::SetDirectionalLightUniforms(cubeShader.get());
    ShaderSystem::SetDirectionalLightUniforms(modelShader.get());
    ShaderSystem::SetCrosshairStaticUniforms(crosshairShader.get());
    ShaderSystem::SetBoxStaticUniforms(cubeShader.get());
    ShaderSystem::SetGrassStaticUniforms(grassShader.get());
    ShaderSystem::SetTreeStaticUniforms(modelShader.get());
    ShaderSystem::SetTrunkStaticUniforms(modelShader.get());
    ShaderSystem::SetRockStaticUniforms(modelShader.get());
    ShaderSystem::SetBushStaticUniforms(modelShader.get());
    ShaderSystem::SetMushroomStaticUniforms(modelShader.get());
    ShaderSystem::SetOutlineStaticUniforms(outlineShader.get());
}

void Game::Run() {
      while (!glfwWindowShouldClose(window)) {
          // clear buffers
          glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
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

    if (!isOrbitMode) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera->KeyboardMovement(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera->KeyboardMovement(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera->KeyboardMovement(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera->KeyboardMovement(RIGHT, deltaTime);
    }
    else {
        static float radius = 40.0f; // is set in entity manager -> floor
        static float angle = 0.0f;
        static constexpr float speed = 0.30f;
        float x = 0.0f + radius * cos(angle);
        float z = 0.0f + radius * sin(angle);
        camera->Position = glm::vec3(x, 8.0f, z);
        camera->Front = glm::normalize(0.0f - camera->Position);
        angle += speed * deltaTime;

    }

    static bool qPressed = false;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && !qPressed) {
        isOrbitMode = !isOrbitMode;
        qPressed = true;
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE)
        qPressed = false;
}

void Game::Update() {
    float currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    PhysicsSystem::update(registry, deltaTime);
    CollisionSystem::update(registry, deltaTime);
}

void Game::Render() {
    ShaderSystem::SetBoxDynamicUniforms(cubeShader.get(), camera);
    ShaderSystem::SetGrassDynamicUniforms(grassShader.get(), camera);
    ShaderSystem::SetTreeDynamicUniforms(modelShader.get(), camera);
    ShaderSystem::SetTrunkDynamicUniforms(modelShader.get(), camera);
    ShaderSystem::SetRockDynamicUniforms(modelShader.get(), camera);
    ShaderSystem::SetBushDynamicUniforms(modelShader.get(), camera);
    ShaderSystem::SetMushroomDynamicUniforms(modelShader.get(), camera);
    ShaderSystem::SetOutlineDynamicUniforms(outlineShader.get(), camera);

    // enable depth testing
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0x00);

    RenderSystem::RenderFloor(registry, cubeShader.get());
    RenderSystem::RenderGrass(registry, grassShader.get());
    RenderSystem::RenderBoxes(registry, cubeShader.get(), outlineShader.get());
    RenderSystem::RenderTree(registry, modelShader.get(), outlineShader.get());
    RenderSystem::RenderTrunk(registry, modelShader.get(), outlineShader.get());
    RenderSystem::RenderRock(registry, modelShader.get(), outlineShader.get());
    RenderSystem::RenderBush(registry, modelShader.get(), outlineShader.get());
    RenderSystem::RenderMushroom(registry, modelShader.get(), outlineShader.get());
     
    // disable depth test for crosshair
    glDisable(GL_DEPTH_TEST);
    RenderSystem::RenderCrosshair(registry, crosshairShader.get()); // render crosshair since we need it with depth test disabled
    
    // reset stencil opeitons to default for proper next iteration and stencil buffer clear
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    glEnable(GL_DEPTH_TEST);
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