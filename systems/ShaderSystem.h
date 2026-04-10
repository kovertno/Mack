#ifndef SHADER_SYSTEM_H
#define SHADER_SYSTEM_H

#include "Shader.hpp"
#include "Camera.hpp"
#include "Game.hpp"
#include "SceneShaders.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ShaderSystem {
private:

public:
	ShaderSystem() = default;
	~ShaderSystem() = default;

	static void CreateMatricesUBO(unsigned int& matricesUBO);
	static void UpdateMatricesUBO(unsigned int& matricesUBO, std::unique_ptr<Camera>& camera);
	static void CreateDataUBO(unsigned int& dataUBO);
	static void UpdateDataUBO(unsigned int& dataUBO, std::unique_ptr<Camera>& camera);
	static void SetDirectionalLightUniforms(Shader* shader);
	static void SetSpotLightStaticUniforms(Shader* shader);
	static void SetSpotLightDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera, bool useSpotLight);
	static void SetPointLightStaticUniforms(Shader* shader);
	static void SetPointLightDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera, entt::registry& registry);
	static void SetCrosshairShaderStaticUniforms(Shader* shader);
	static void SetOutlineShaderStaticUniforms(Shader* shader);
	static void SetPostProcessing(Shader* shader, unsigned int& VAO, unsigned int& VBO);
	static void SetSkyboxVAO(unsigned int& VAO, unsigned int& VBO);
	static void SetStaticUniforms(SceneShaders& sceneShaders, unsigned int& framebufferVAO, unsigned int& framebufferVBO, unsigned int& skyboxVAO, unsigned int& skyboxVBO);
	static void SetDynamicUniforms(SceneShaders& sceneShaders, std::unique_ptr<Camera>& camera, entt::registry& registry, bool useSpotLight, unsigned int& matricesUBO, unsigned int& dataUBO);
};

#endif