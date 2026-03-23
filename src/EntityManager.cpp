#include "EntityManager.hpp"

#include "TransformComponent.hpp"
#include "BoxMeshComponent.hpp"
#include "CrosshairMeshComponent.hpp"
#include "MaterialComponent.hpp"
#include "GrassMeshComponent.hpp"
#include "ModelMeshComponent.hpp"
#include "GravityComponent.h"
#include "RigidBodyComponent.h"
#include "BoxColliderComponent.hpp"
#include "KnockBackComponent.hpp"
#include "StaticColliderComponent.hpp"
#include "TreeModelComponent.hpp"
#include "TrunkModelComponent.hpp"
#include "RockModelComponent.hpp"
#include "BushModelComponent.hpp"
#include "MushroomModelComponent.hpp"
#include "OutlineComponent.hpp"
#include "Model.hpp"

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <vector>
#include <random>
#include <string>

void EntityManager::CreateCrosshair(unsigned int VAO) {
    entt::entity crosshairEntity = m_registry.create();
    auto& crosshairMesh = m_registry.emplace<CrosshairMeshComponent>(crosshairEntity);
    crosshairMesh.VAO = VAO;
    crosshairMesh.numOfVertices = 6;
}

void EntityManager::CreateEnemies(unsigned int VAO, unsigned int numOfEnemies) {

    // enemy positions are hardcoded for now, this needs a change later, i think?
    std::vector<glm::vec3> enemyPos{
        glm::vec3{ 3.0f, 0.25f, -8.0f },
        glm::vec3{ -6.0f, 0.25f, -10.0f },
        glm::vec3{ 0.0f, 0.25f, -2.0f }
    };

    for (glm::vec3& pos : enemyPos) {
        takenPositions.push_back(pos);
    }

    for (unsigned int i = 0; i < numOfEnemies; ++i) {
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
        auto& box1Rigidbody = m_registry.emplace<RigidBodyComponent>(enemyEntity, glm::vec3(0.0f), glm::vec3(0.0f),2.0f);
        auto& box1Collider = m_registry.emplace<BoxColliderComponent>(enemyEntity);
        auto& box1KnockBack = m_registry.emplace<KnockBackComponent>(enemyEntity);
        box1Mesh.VAO = VAO;
        box1Mesh.numOfVertices = 36;
        m_registry.emplace<OutlineComponent>(enemyEntity);
    }
}

void EntityManager::CreateFloor(unsigned int VAO) {
    entt::entity floorEntity = m_registry.create();
    auto& floorTransform = m_registry.emplace<TransformComponent>(floorEntity);
    floorTransform.position = glm::vec3(0.0f, -0.6f, 0.0f);
    floorTransform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    floorTransform.scale = glm::vec3(60.0f, 0.2f, 60.0f);
    auto& floorMaterial = m_registry.emplace<MaterialComponent>(floorEntity);
    floorMaterial.ambient = glm::vec3(0.0f, 0.3f, 0.0f);
    floorMaterial.diffuse = glm::vec3(0.0f, 0.7f, 0.0f);
    floorMaterial.specular = glm::vec3(0.6f, 0.6f, 0.6f);
    floorMaterial.shininess = 8.0f;
    auto& floorMesh = m_registry.emplace<BoxMeshComponent>(floorEntity);
    auto& floorCollider = m_registry.emplace<BoxColliderComponent>(floorEntity);
    auto& floorStaticColl = m_registry.emplace<StaticColliderComponent>(floorEntity);
    floorMesh.VAO = VAO;
    floorMesh.numOfVertices = 36;
}

void EntityManager::CreateGrass(unsigned int VAO) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> disX(-29.0f, 29.0f);
    std::uniform_real_distribution<float> disZ(-29.0f, 29.0f);

    constexpr uint16_t numOfGrass = 500;

    std::vector<glm::vec3> grassPos{};

    for (uint16_t i = 0; i < numOfGrass; ++i) {
        float randomX = disX(gen);
        float randomZ = disZ(gen);
        grassPos.push_back(glm::vec3(randomX, 0.0f, randomZ));
    }

    // rotation needs better calculation in the future, it causes grass to rotate slightly along the wrong axis sometimes
    std::uniform_real_distribution<float> rotX(-20.0f, 20.0f);
    std::uniform_real_distribution<float> rotY(-180.0f, 180.0f);

    for (uint16_t i = 0; i < numOfGrass; ++i) {
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

void EntityManager::CreateBackpackModel() {
    entt::entity backpackEntity = m_registry.create();
    auto& backpackTransform = m_registry.emplace<TransformComponent>(backpackEntity);
    backpackTransform.position = glm::vec3(2.0f, 1.0f, 0.0f);
    backpackTransform.scale = glm::vec3(0.5f, 0.5f, 0.5f);
    auto& backpackModel = m_registry.emplace<ModelMeshComponent>(backpackEntity);
    backpackModel.model.LoadModel("resources/models/backpack/backpack.obj");
}

void EntityManager::CreateTreeModel() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> disX(-29.0f, 29.0f);
    std::uniform_real_distribution<float> disZ(-29.0f, 29.0f);

    constexpr uint16_t numOfTrees = 12;

    std::vector<glm::vec3> treePositions{};

    for (uint16_t i = 0; i < numOfTrees; ++i) {
        glm::vec3 position{};
        do {
            float randomX = disX(gen);
            float randomZ = disZ(gen);
            position = glm::vec3(randomX, -0.5f, randomZ);
        } while (IsPositionTaken(position));

        treePositions.push_back(position);
        takenPositions.push_back(position);
    }

    std::uniform_real_distribution<float> rotZ(-180.0f, 180.0f);

    for (glm::vec3& treePos : treePositions) {
        entt::entity treeEntity = m_registry.create();
        auto& treeTransform = m_registry.emplace<TransformComponent>(treeEntity);
        treeTransform.position = treePos;
        treeTransform.scale = glm::vec3(1.5f, 1.5f, 1.5f);
        treeTransform.rotation = glm::vec3(-90.0f, 0.0f, rotZ(gen));
        auto& treeModel = m_registry.emplace<ModelMeshComponent>(treeEntity);
        treeModel.model.LoadModel("resources/models/low_poly_trees_grass_and_rocks/tree_round.gltf");
        m_registry.emplace<TreeModelComponent>(treeEntity);
        m_registry.emplace<OutlineComponent>(treeEntity);
    }
}

void EntityManager::CreateTrunkModel() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> disX(-29.0f, 29.0f);
    std::uniform_real_distribution<float> disZ(-29.0f, 29.0f);

    constexpr uint16_t numOfTrunks = 5;

    std::vector<glm::vec3> trunkPositions{};

    for (uint16_t i = 0; i < numOfTrunks; ++i) {
        glm::vec3 position{};
        do {
            float randomX = disX(gen);
            float randomZ = disZ(gen);
            position = glm::vec3(randomX, -0.5f, randomZ);
        } while (IsPositionTaken(position));

        trunkPositions.push_back(position);
        takenPositions.push_back(position);
    }

    std::uniform_real_distribution<float> rotZ(-180.0f, 180.0f);

    int i = 0;
    for (glm::vec3& trunkPos : trunkPositions) {
        entt::entity trunkEntity = m_registry.create();
        auto& trunkTransform = m_registry.emplace<TransformComponent>(trunkEntity);
        trunkTransform.position = trunkPos;
        trunkTransform.rotation = glm::vec3(-90.0f, 0.0f, rotZ(gen));
        auto& trunkModel = m_registry.emplace<ModelMeshComponent>(trunkEntity);
        if (i % 2 == 0)
            trunkModel.model.LoadModel("resources/models/low_poly_trees_grass_and_rocks/trunk1.gltf");
        else
            trunkModel.model.LoadModel("resources/models/low_poly_trees_grass_and_rocks/trunk2.gltf");
        m_registry.emplace<TrunkModelComponent>(trunkEntity);
        m_registry.emplace<OutlineComponent>(trunkEntity);

        ++i;
    }
}

void EntityManager::CreateRockModel() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> disX(-29.0f, 29.0f);
    std::uniform_real_distribution<float> disZ(-29.0f, 29.0f);

    constexpr uint16_t numOfRocks = 10;

    std::vector<glm::vec3> rockPositions{};

    for (uint16_t i = 0; i < numOfRocks; ++i) {
        glm::vec3 position{};
        do {
            float randomX = disX(gen);
            float randomZ = disZ(gen);
            position = glm::vec3(randomX, -0.5f, randomZ);
        } while (IsPositionTaken(position));

        rockPositions.push_back(position);
        takenPositions.push_back(position);
    }

    std::uniform_real_distribution<float> rotZ(-180.0f, 180.0f);
    std::uniform_real_distribution<float> modNum(1.0f, 4.99f);

    for (glm::vec3& rockPos : rockPositions) {
        entt::entity rockEntity = m_registry.create();
        auto& rockTransform = m_registry.emplace<TransformComponent>(rockEntity);
        rockTransform.position = rockPos;
        rockTransform.rotation = glm::vec3(-90.0f, 0.0f, rotZ(gen));
        auto& rockModel = m_registry.emplace<ModelMeshComponent>(rockEntity);
        rockModel.model.LoadModel("resources/models/low_poly_trees_grass_and_rocks/rock" + std::to_string(static_cast<int>(modNum(gen))) + ".gltf");
        m_registry.emplace<RockModelComponent>(rockEntity);
        m_registry.emplace<OutlineComponent>(rockEntity);
    }
}

void EntityManager::CreateBushModel() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> disX(-29.0f, 29.0f);
    std::uniform_real_distribution<float> disZ(-29.0f, 29.0f);

    constexpr uint16_t numOfBushes = 10;

    std::vector<glm::vec3> bushPositions{};

    for (uint16_t i = 0; i < numOfBushes; ++i) {
        glm::vec3 position{};
        do {
            float randomX = disX(gen);
            float randomZ = disZ(gen);
            position = glm::vec3(randomX, -0.5f, randomZ);
        } while (IsPositionTaken(position));

        bushPositions.push_back(position);
        takenPositions.push_back(position);
    }

    std::uniform_real_distribution<float> rotZ(-180.0f, 180.0f);
    std::uniform_real_distribution<float> modNum(1.0f, 2.99f);

    for (glm::vec3& bushPos : bushPositions) {
        entt::entity bushEntity = m_registry.create();
        auto& bushTransform = m_registry.emplace<TransformComponent>(bushEntity);
        bushTransform.position = bushPos;
        bushTransform.rotation = glm::vec3(-90.0f, 0.0f, rotZ(gen));
        auto& rockModel = m_registry.emplace<ModelMeshComponent>(bushEntity);
        rockModel.model.LoadModel("resources/models/low_poly_trees_grass_and_rocks/bush" + std::to_string(static_cast<int>(modNum(gen))) + ".gltf");
        m_registry.emplace<BushModelComponent>(bushEntity);
        m_registry.emplace<OutlineComponent>(bushEntity);
    }
}

void EntityManager::CreateMushroomModel() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> disX(-29.0f, 29.0f);
    std::uniform_real_distribution<float> disZ(-29.0f, 29.0f);

    constexpr uint16_t numOfMushrooms = 15;

    std::vector<glm::vec3> mushroomPositions{};

    for (uint16_t i = 0; i < numOfMushrooms; ++i) {
        glm::vec3 position{};
        do {
            float randomX = disX(gen);
            float randomZ = disZ(gen);
            position = glm::vec3(randomX, -0.5f, randomZ);
        } while (IsPositionTaken(position));

        mushroomPositions.push_back(position);
        takenPositions.push_back(position);
    }

    std::uniform_real_distribution<float> rotZ(-180.0f, 180.0f);
    std::uniform_real_distribution<float> modNum(1.0f, 2.99f);

    for (glm::vec3& mushroomPos : mushroomPositions) {
        entt::entity mushroomEntity = m_registry.create();
        auto& mushroomTransform = m_registry.emplace<TransformComponent>(mushroomEntity);
        mushroomTransform.position = mushroomPos;
        mushroomTransform.rotation = glm::vec3(-90.0f, 0.0f, rotZ(gen));

        int numOfModel = static_cast<int>(modNum(gen));
        if (numOfModel == 1)
            mushroomTransform.scale = glm::vec3(0.9f, 0.9f, 0.9f);
        else
            mushroomTransform.scale = glm::vec3(0.1f, 0.1f, 0.1f);

        auto& mushroomModel = m_registry.emplace<ModelMeshComponent>(mushroomEntity);
        mushroomModel.model.LoadModel("resources/models/low_poly_trees_grass_and_rocks/mushroom" + std::to_string(numOfModel) + ".gltf");
        m_registry.emplace<MushroomModelComponent>(mushroomEntity);
        m_registry.emplace<OutlineComponent>(mushroomEntity);
    }
}

bool EntityManager::IsPositionTaken(glm::vec3 position) {
    for (glm::vec3& pos : takenPositions) {
        float dist = glm::distance(pos, position);
        if (dist < 4.0f)
            return true;
    }

    return false;
}