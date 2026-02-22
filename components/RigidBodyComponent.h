#pragma once

#include <glm/glm.hpp>

struct RigidBodyComponent
{
	glm::vec3 velocity;
	glm::vec3 acceleration;
	bool isStatic;
	RigidBodyComponent(glm::vec3 velocity = glm::vec3(0.0f), glm::vec3 acceleration = glm::vec3(0.0f), bool isStatic = false) : 
		velocity(velocity),
		acceleration(acceleration),
		isStatic(isStatic) {}
};