#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "TransformComponent.hpp"
#include "BoxMeshComponent.hpp"
#include "CrosshairMeshComponent.hpp"
#include "MaterialComponent.hpp"
#include "GrassMeshComponent.hpp"

#include <entt/entt.hpp>

class EntityManager {
private:
	entt::registry& m_registry;
public:
	EntityManager(entt::registry& registry) : m_registry(registry) {}
	~EntityManager() = default;

	// crosshair shouldnt be an entity but im too lazy to change it
	void CreateCrosshair(unsigned int VAO);
	void CreateEnemies(unsigned int VAO, unsigned int numOfEnemies);
	void CreateFloor(unsigned int VAO);
	void CreateGrass(unsigned int VAO);
};

#endif