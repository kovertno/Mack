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
#include "FireflySystem.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <entt/entt.hpp>

#include <iostream>
#include <bitset>

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
    skyboxShader = std::make_unique<Shader>("resources/shaders/skyboxShader.vs", "resources/shaders/skyboxShader.fs");

    sceneShaders = {cubeShader.get(), crosshairShader.get(), grassShader.get(), modelShader.get(), outlineShader.get(), postProcessingShader.get(), skyboxShader.get()};

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
    // flashlight
    entityManager->CreateFlashlightModel(camera);
    // fireflies
    entityManager->CreateFireflies();

    framebuffer = std::make_unique<Framebuffer>();
    skyboxTexture = TextureUtils::LoadCubemap(skyboxFaces);
    ShaderSystem::SetStaticUniforms(sceneShaders, framebufferVAO, framebufferVBO, skyboxVAO, skyboxVBO);
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

    if (!settings.test(ORBIT_MODE)) {
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
        camera->Orbit(radius, angle, deltaTime);
        angle += speed * deltaTime;
    }

    static bool qPressed = false;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && !qPressed) {
        settings.flip(ORBIT_MODE);
        qPressed = true;
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE)
        qPressed = false;

    static bool ePressed = false;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && !ePressed) {
        settings.flip(POST_PROCESSING);
        ePressed = true;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE)
        ePressed = false;

    static bool fPressed = false;
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !fPressed) {
        settings.flip(FLASHLIGHT);
        fPressed = true;
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
        fPressed = false;

    if (settings.test(FLASHLIGHT)) {
        static bool mousePressed = false;
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && !mousePressed) {
            settings.flip(FLASHLIGHT_ON);
            mousePressed = true;
        }
        
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
            mousePressed = false;
    }
}

void Game::Update() {
    float currentFrame = (float)glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    PhysicsSystem::update(registry, deltaTime);
    CollisionSystem::update(registry, deltaTime);
    FireflySystem::Update(registry);
}

void Game::Render() {
    if (!settings.test(POST_PROCESSING)) {
        // clear buffers
        glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        ShaderSystem::SetDynamicUniforms(sceneShaders, camera, registry, settings.test(FLASHLIGHT_ON));


        // enable depth testing
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilMask(0x00);

        RenderSystem::RenderScene(registry, sceneShaders, skyboxVAO, skyboxTexture);

        if (settings.test(FLASHLIGHT)) {
            RenderSystem::RenderFlashlight(registry, modelShader.get(), camera);
        }
        else {
            settings.reset(FLASHLIGHT_ON);
        }

        // disable depth test for crosshair
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);
        RenderSystem::RenderCrosshair(registry, crosshairShader.get()); // render crosshair since we need it with depth test disabled

        // reset stencil opeitons to default for proper next iteration and stencil buffer clear
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
    }
    else {
        framebuffer->Bind();
        // clear buffers
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        ShaderSystem::SetDynamicUniforms(sceneShaders, camera, registry, settings.test(FLASHLIGHT_ON));

        // enable depth testing
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilMask(0x00);

        RenderSystem::RenderScene(registry, sceneShaders, skyboxVAO, skyboxTexture);

        if (settings.test(FLASHLIGHT)) {
            RenderSystem::RenderFlashlight(registry, modelShader.get(), camera);
        }
        else {
            settings.reset(FLASHLIGHT_ON);
        }

        // reset stencil opeitons to default for proper next iteration and stencil buffer clear
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        glEnable(GL_DEPTH_TEST);

        framebuffer->Unbind();
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // disable depth
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_STENCIL_TEST);
        RenderSystem::RenderPostProcessing(postProcessingShader.get(), framebufferVAO, framebuffer->GetTexture());   
        RenderSystem::RenderCrosshair(registry, crosshairShader.get()); 

        // reset stencil opeitons to default for proper next iteration and stencil buffer clear
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
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