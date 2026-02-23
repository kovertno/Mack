#ifndef GAME_H
#define GAME_H

#include "Game.hpp"
#include "Window.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

#include "TransformComponent.hpp"
#include "BoxMeshComponent.hpp"

#include "RenderSystem.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <entt/entt.hpp>

#include <iostream>


class Game {
private:
	GLFWwindow* window = nullptr;
	std::unique_ptr<Camera> camera = nullptr;
	std::unique_ptr<Shader> testShader = nullptr;
	// delta Time
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	// starting mouse position
	inline static float lastX = 600;
	inline static float lastY = 400;
	// is it the first time loading the mouse position
	inline static bool isFirstMouse = true;
public:
	entt::registry registry;

	static constexpr unsigned int SCR_WIDTH = 1200;
	static constexpr unsigned int SCR_HEIGHT = 800;

	Game() = default;
	~Game() = default;

	void Init();
	void Run();

	void ProcessInput();
	void Update();
	void Render();

	void SetCallbacks();
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn);
};

#endif