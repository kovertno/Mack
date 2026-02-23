#pragma once

#include <glm/glm.hpp>

struct GravityComponent 
{
	glm::vec3 gravityPull;
	GravityComponent(glm::vec3 gravityPull = glm::vec3(0.0f, -9.81f, 0.0f)) : 
		gravityPull(gravityPull) {}
};