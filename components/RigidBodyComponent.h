#pragma once

#include <glm/glm.hpp>

struct RigidBodyComponent
{
	glm::vec3 velocity;
	glm::vec3 acceleration;
	float mass;
	bool isStatic;
	RigidBodyComponent(glm::vec3 velocity = glm::vec3(0.0f), glm::vec3 acceleration = glm::vec3(0.0f),float mass = 1.0f) : 
		velocity(velocity),
		acceleration(acceleration),
		mass(mass) {}
};