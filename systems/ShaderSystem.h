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

	static void SetDirectionalLightUniforms(Shader* shader);
	static void SetSpotLightStaticUniforms(Shader* shader);
	static void SetSpotLightDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera, bool useSpotLight);
	static void SetPointLightStaticUniforms(Shader* shader);
	static void SetPointLightDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera, entt::registry& registry);
	static void SetCubeShaderStaticUniforms(Shader* shader);
	static void SetCubeShaderDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera);
	static void SetCrosshairShaderStaticUniforms(Shader* shader);
	static void SetGrassShaderStaticUniforms(Shader* shader);
	static void SetGrassShaderDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera);
	static void SetModelShaderStaticUniforms(Shader* shader);
	static void SetModelShaderDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera);
	static void SetOutlineShaderStaticUniforms(Shader* shader);
	static void SetOutlineShaderDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera);
	static void SetPostProcessing(Shader* shader, unsigned int& VAO, unsigned int& VBO);
	static void SetSkyboxVAO(unsigned int& VAO, unsigned int& VBO);
	static void SetSkyboxStaticUnifoms(Shader* shader);
	static void SetSkyboxDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera);
	static void SetStaticUniforms(SceneShaders& sceneShaders, unsigned int& framebufferVAO, unsigned int& framebufferVBO, unsigned int& skyboxVAO, unsigned int& skyboxVBO);
	static void SetDynamicUniforms(SceneShaders& sceneShaders, std::unique_ptr<Camera>& camera, entt::registry& registry, bool useSpotLight);
};

#endif