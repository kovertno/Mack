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
	static void SetBoxStaticUniforms(Shader* shader);
	static void SetBoxDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera);
	static void SetCrosshairStaticUniforms(Shader* shader);
	static void SetGrassStaticUniforms(Shader* shader);
	static void SetGrassDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera);
	static void SetTreeStaticUniforms(Shader* shader);
	static void SetTreeDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera);
	static void SetTrunkStaticUniforms(Shader* shader);
	static void SetTrunkDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera);
	static void SetRockStaticUniforms(Shader* shader);
	static void SetRockDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera);
	static void SetBushStaticUniforms(Shader* shader);
	static void SetBushDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera);
	static void SetMushroomStaticUniforms(Shader* shader);
	static void SetMushroomDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera);
	static void SetOutlineStaticUniforms(Shader* shader);
	static void SetOutlineDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera);
	static void SetPostProcessing(Shader* shader, unsigned int& VAO, unsigned int& VBO);
	static void SetSkyboxVAO(unsigned int& VAO, unsigned int& VBO);
	static void SetSkyboxStaticUnifoms(Shader* shader);
	static void SetSkyboxDynamicUniforms(Shader* shader, std::unique_ptr<Camera>& camera);
	static void SetStaticUniforms(SceneShaders& sceneShaders, unsigned int& framebufferVAO, unsigned int& framebufferVBO, unsigned int& skyboxVAO, unsigned int& skyboxVBO);
	static void SetDynamicUniforms(SceneShaders& sceneShaders, std::unique_ptr<Camera>& camera);
};

#endif