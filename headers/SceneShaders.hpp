#ifndef SCENE_SHADERS_H
#define SCENE_SHADERS_H

#include "Shader.hpp"

struct SceneShaders {
	Shader* cubeShader = nullptr;
	Shader* crosshairShader = nullptr;
	Shader* grassShader = nullptr;
	Shader* modelShader = nullptr;
	Shader* outlineShader = nullptr;
	Shader* postProcessingShader = nullptr;
	Shader* skyboxShader = nullptr;
};

#endif