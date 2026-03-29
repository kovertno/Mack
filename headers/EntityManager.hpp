#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "TransformComponent.hpp"
#include "BoxMeshComponent.hpp"
#include "CrosshairMeshComponent.hpp"
#include "MaterialComponent.hpp"
#include "GrassMeshComponent.hpp"
#include "ModelMeshComponent.hpp"
#include "TreeModelComponent.hpp"
#include "TrunkModelComponent.hpp"
#include "RockModelComponent.hpp"
#include "BushModelComponent.hpp"
#include "MushroomModelComponent.hpp"
#include "Camera.hpp"

#include <entt/entt.hpp>

class EntityManager {
private:
	entt::registry& m_registry;
	std::vector<glm::vec3> takenPositions;

	bool IsPositionTaken(glm::vec3 position);
public:
	EntityManager(entt::registry& registry) : m_registry(registry) {}
	~EntityManager() = default;

	// crosshair shouldnt be an entity but im too lazy to change it
	void CreateCrosshair(unsigned int VAO);
	void CreateEnemies(unsigned int VAO, unsigned int numOfEnemies);
	void CreateFloor(unsigned int VAO);
	void CreateGrass(unsigned int VAO);
	void CreateBackpackModel();
	void CreateTreeModel();
	void CreateTrunkModel();
	void CreateRockModel();
	void CreateBushModel();
	void CreateMushroomModel();
	void CreateModelMap();
	void CreateFlashlightModel(std::unique_ptr<Camera>& camera);
};

#endif