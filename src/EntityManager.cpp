#include "EntityManager.hpp"

#include "TransformComponent.hpp"
#include "BoxMeshComponent.hpp"
#include "CrosshairMeshComponent.hpp"
#include "MaterialComponent.hpp"
#include "GrassMeshComponent.hpp"
#include "BoxColliderComponent.hpp"
#include "GravityComponent.h"
#include "KnockBackComponent.hpp"
#include "RigidBodyComponent.h"

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <random>

void EntityManager::CreateCrosshair(unsigned int VAO) {
    entt::entity crosshairEntity = m_registry.create();
    auto& crosshairMesh = m_registry.emplace<CrosshairMeshComponent>(crosshairEntity);
    crosshairMesh.VAO = VAO;
    crosshairMesh.numOfVertices = 6;
}

void EntityManager::CreateEnemies(unsigned int VAO, unsigned int numOfEnemies) {

    // enemy positions are hardcoded for now, this needs a change later, i think?
    std::vector<glm::vec3> enemyPos{
        glm::vec3{ 3.0f, 2.0f, -8.0f },
        glm::vec3{ -6.0f, 2.0f, -10.0f },
        glm::vec3{ 0.0f, 2.0f, -2.0f }
    };

    for (unsigned int i = 0; i < numOfEnemies; i++) {
        entt::entity enemyEntity = m_registry.create();
        auto& box1Transform = m_registry.emplace<TransformComponent>(enemyEntity);
        box1Transform.position = enemyPos[i];
        box1Transform.scale.y = 1.5f;
        auto& cube1Material = m_registry.emplace<MaterialComponent>(enemyEntity);
        cube1Material.ambient = glm::vec3(0.3f, 0.0f, 0.0f);
        cube1Material.diffuse = glm::vec3(0.7f, 0.0f, 0.0f);
        cube1Material.specular = glm::vec3(1.0f, 1.0f, 1.0f);
        auto& box1Mesh = m_registry.emplace<BoxMeshComponent>(enemyEntity);
        auto& box1Gravity = m_registry.emplace<GravityComponent>(enemyEntity, glm::vec3(0.0f, -9.0f, 0.0f));
        auto& box1Rigidbody = m_registry.emplace<RigidBodyComponent>(enemyEntity, glm::vec3(0.0f), glm::vec3(0.0f), false);
        auto& box1Collider = m_registry.emplace<BoxColliderComponent>(enemyEntity);
        auto& box1KnockBack = m_registry.emplace<KnockBackComponent>(enemyEntity);
        box1Mesh.VAO = VAO;
        box1Mesh.numOfVertices = 36;
    }
}

void EntityManager::CreateFloor(unsigned int VAO) {
    entt::entity floorEntity = m_registry.create();
    auto& floorTransform = m_registry.emplace<TransformComponent>(floorEntity);
    floorTransform.position = glm::vec3(0.0f, -0.6f, 0.0f);
    floorTransform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    floorTransform.scale = glm::vec3(30.0f, 0.2f, 30.0f);
    auto& floorMaterial = m_registry.emplace<MaterialComponent>(floorEntity);
    floorMaterial.ambient = glm::vec3(0.0f, 0.3f, 0.0f);
    floorMaterial.diffuse = glm::vec3(0.0f, 0.7f, 0.0f);
    floorMaterial.specular = glm::vec3(0.6f, 0.6f, 0.6f);
    floorMaterial.shininess = 8.0f;
    auto& floorMesh = m_registry.emplace<BoxMeshComponent>(floorEntity);
    auto& floorCollider = m_registry.emplace<BoxColliderComponent>(floorEntity);
    auto& floorRigidbody = m_registry.emplace<RigidBodyComponent>(floorEntity, glm::vec3(0.0f), glm::vec3(0.0f), true);
    floorMesh.VAO = VAO;
    floorMesh.numOfVertices = 36;
}

void EntityManager::CreateGrass(unsigned int VAO) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> disX(-13.0f, 13.0f);
    std::uniform_real_distribution<float> disZ(-13.0f, 13.0f);

    uint16_t numOfGrass = 450;

    std::vector<glm::vec3> grassPos{};

    for (uint16_t i = 0; i < numOfGrass; i++) {
        float randomX = disX(gen);
        float randomZ = disZ(gen);
        grassPos.push_back(glm::vec3(randomX, 0.0f, randomZ));
    }

    // rotation needs better calculation in the future, it causes grass to rotate slightly along the wrong axis sometimes
    std::uniform_real_distribution<float> rotX(-20.0f, 20.0f);
    std::uniform_real_distribution<float> rotY(-180.0f, 180.0f);

    for (uint16_t i = 0; i < numOfGrass; i++) {
        entt::entity grassEntity = m_registry.create();
        auto& grassTransform = m_registry.emplace<TransformComponent>(grassEntity);
        grassTransform.position = grassPos[i];
        grassTransform.position.y = -0.4f;
        grassTransform.rotation.x = rotX(gen);//0-30
        grassTransform.rotation.y = rotY(gen);//0-180
        grassTransform.scale = glm::vec3(0.2f, 0.2f, 0.2f);
        auto& grassMesh = m_registry.emplace<GrassMeshComponent>(grassEntity);
        grassMesh.VAO = VAO;
        grassMesh.numOfVertices = 3;
    }
}