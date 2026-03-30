#ifndef GAME_H
#define GAME_H

#include "Window.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "EntityManager.hpp"
#include "Framebuffer.hpp"
#include "SceneShaders.hpp"

#include "RenderSystem.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <entt/entt.hpp>

#include <iostream>
#include <bitset>

class Game {
private:
	GLFWwindow* window = nullptr;
	std::unique_ptr<EntityManager> entityManager = nullptr;
	std::unique_ptr<Camera> camera = nullptr;
	std::unique_ptr<Shader> cubeShader = nullptr;
	std::unique_ptr<Shader> crosshairShader = nullptr;
	std::unique_ptr<Shader> grassShader = nullptr;
	std::unique_ptr<Shader> modelShader = nullptr;
	std::unique_ptr<Shader> outlineShader = nullptr;
	std::unique_ptr<Shader> postProcessingShader = nullptr;
	std::unique_ptr<Shader> skyboxShader = nullptr;

	SceneShaders sceneShaders;

	// delta Time
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	// starting mouse position
	inline static float lastX = 600;
	inline static float lastY = 400;
	// is it the first time loading the mouse position
	inline static bool isFirstMouse = true;

	enum SettingsBits {
		ORBIT_MODE = 0,
		POST_PROCESSING = 1,
		FLASHLIGHT = 2,
		FLASHLIGHT_ON = 3
	};
	std::bitset<8> settings{0b0000'0000};

	unsigned int cubeVAO;
	unsigned int cubeVBO;

	unsigned int crosshairVAO;
	unsigned int crosshairVBO;

	unsigned int grassVAO;
	unsigned int grassVBO;

	std::unique_ptr<Framebuffer> framebuffer = nullptr;
	unsigned int framebufferVAO;
	unsigned int framebufferVBO;

	unsigned int skyboxVAO;
	unsigned int skyboxVBO;
	std::vector<std::string> skyboxFaces
	{
		"resources/textures/skybox/right.png",
		"resources/textures/skybox/left.png",
		"resources/textures/skybox/top.png",
		"resources/textures/skybox/bottom.png",
		"resources/textures/skybox/front.png",
		"resources/textures/skybox/back.png"
	};
	unsigned int skyboxTexture;

public:
	entt::registry registry;

	static constexpr unsigned int SCR_WIDTH = 1200;
	static constexpr unsigned int SCR_HEIGHT = 800;

	Game() = default;
	~Game() {
		glDeleteVertexArrays(1, &cubeVAO);
		glDeleteBuffers(1, &cubeVBO);

		glDeleteVertexArrays(1, &crosshairVAO);
		glDeleteBuffers(1, &crosshairVBO);

		glDeleteVertexArrays(1, &grassVAO);
		glDeleteBuffers(1, &grassVBO);

		glDeleteVertexArrays(1, &framebufferVAO);
		glDeleteBuffers(1, &framebufferVBO);

		glDeleteVertexArrays(1, &skyboxVAO);
		glDeleteBuffers(1, &skyboxVBO);

		glDeleteTextures(1, &skyboxTexture);

		glfwTerminate();
	}

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