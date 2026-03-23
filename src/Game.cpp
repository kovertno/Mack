#include "Game.hpp"
#include "Window.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "EntityManager.hpp"
#include "ModelMesh.hpp"
#include "Model.hpp"
#include "Framebuffer.hpp"
#include "SceneShaders.hpp"
#include "TextureUtils.hpp"

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
	postProcessingShader = std::make_unique<Shader>("resources/shaders/postProcessingShader.vs", "resources/shaders/postProcessingShader.fs");

    sceneShaders = {cubeShader.get(), crosshairShader.get(), grassShader.get(), modelShader.get(), outlineShader.get(), postProcessingShader.get()};

    CrosshairMeshComponent::SetCrosshairVAO(crosshairVAO, crosshairVBO);
    BoxMeshComponent::SetBoxVAO(cubeVAO, cubeVBO);
    GrassMeshComponent::SetGrassVAO(grassVAO, grassVBO);

    cubemapTexture = TextureUtils::LoadCubemap(skyboxFaces);

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

    framebuffer = std::make_unique<Framebuffer>();
    ShaderSystem::SetStaticUniforms(sceneShaders, framebufferVAO, framebufferVBO);
}

void Game::Run() {
      while (!glfwWindowShouldClose(window)) {
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
        static float radius = 40.0f; // taken from entity manager -> floor
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

    static bool ePressed = false;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && !ePressed) {
        isPostProcessingEnabled = !isPostProcessingEnabled;
        ePressed = true;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE)
        ePressed = false;
}

void Game::Update() {
    float currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    PhysicsSystem::update(registry, deltaTime);
    CollisionSystem::update(registry, deltaTime);
}

void Game::Render() {
    if (!isPostProcessingEnabled) {
        // clear buffers
        glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        ShaderSystem::SetDynamicUniforms(sceneShaders, camera);


        // enable depth testing
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilMask(0x00);

        RenderSystem::RenderScene(registry, sceneShaders);

        // disable depth test for crosshair
        glDisable(GL_DEPTH_TEST);
        RenderSystem::RenderCrosshair(registry, crosshairShader.get()); // render crosshair since we need it with depth test disabled
    }
    else {
        framebuffer->Bind();
        // clear buffers
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        ShaderSystem::SetDynamicUniforms(sceneShaders, camera);

        // enable depth testing
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilMask(0x00);

        RenderSystem::RenderScene(registry, sceneShaders);

        // reset stencil opeitons to default for proper next iteration and stencil buffer clear
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        glEnable(GL_DEPTH_TEST);

        framebuffer->Unbind();
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // disable depth
        glDisable(GL_DEPTH_TEST);
        RenderSystem::RenderPostProcessing(postProcessingShader.get(), framebufferVAO, framebuffer->GetTexture());   
        RenderSystem::RenderCrosshair(registry, crosshairShader.get()); 

        // reset stencil opeitons to default for proper next iteration and stencil buffer clear
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        glEnable(GL_DEPTH_TEST);
    }
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