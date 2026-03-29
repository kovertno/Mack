#include "TerrainCollisionSystem.hpp"
#include "HeightfieldColliderComponent.hpp"
#include "ModelMeshComponent.hpp"
#include "TransformComponent.hpp"
#include "RigidBodyComponent.h"
#include "GravityComponent.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <limits>

bool TerrainCollisionSystem::rayTriangleIntersect(const glm::vec3& orig, const glm::vec3& dir, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, float& outT) {
    const float EPSILON = 1e-7f;
    glm::vec3 edge1 = v1 - v0;
    glm::vec3 edge2 = v2 - v0;
    glm::vec3 h = glm::cross(dir, edge2);
    float a = glm::dot(edge1, h);
    if (a > -EPSILON && a < EPSILON) return false;

    float f = 1.0f / a;
    glm::vec3 s = orig - v0;
    float u = f * glm::dot(s, h);
    if (u < 0.0f || u > 1.0f) return false;

    glm::vec3 q = glm::cross(s, edge1);
    float v = f * glm::dot(dir, q);
    if (v < 0.0f || u + v > 1.0f) return false;

    outT = f * glm::dot(edge2, q);
    return outT > EPSILON;
}

void TerrainCollisionSystem::bake(entt::registry& registry) {
    auto view = registry.view<TransformComponent, ModelMeshComponent, HeightfieldColliderComponent>();

    view.each([](auto& transform, auto& modelMeshComp, auto& heightfield)
        {
            if (heightfield.baked) return;
            heightfield.worldTriangles.clear();

            //world space matrix
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, transform.position);
            model = glm::scale(model, transform.scale);

            for (auto& mesh : modelMeshComp.model.GetMeshes())
            {
                auto& verts = mesh.vertices;
                auto& indices = mesh.indices;

                for (size_t i = 0; i + 2 < mesh.indices.size(); i += 3)
                {
                    Triangle tri;
                    tri.a = model * glm::vec4(mesh.vertices[mesh.indices[i]].Position, 1.0f);
                    tri.b = model * glm::vec4(mesh.vertices[mesh.indices[i + 1]].Position, 1.0f);
                    tri.c = model * glm::vec4(mesh.vertices[mesh.indices[i + 2]].Position, 1.0f);
                    heightfield.worldTriangles.push_back(tri);
                }
            }
			heightfield.baked = true;

			printf("Baked heightfield collider with %zu triangles\n", heightfield.worldTriangles.size());
        });
}

void TerrainCollisionSystem::update(entt::registry& registry, float deltaTime) {
	auto terrains = registry.view<HeightfieldColliderComponent>();
	auto dynamics = registry.view<TransformComponent, RigidBodyComponent>();

    dynamics.each([&](auto& transform, auto& rigidBody)
        {
            if (rigidBody.isStatic) return;
			//Start ray from above the object and point downwards
			glm::vec3 rayOrigin = transform.position + glm::vec3(0.0f, 20.0f, 0.0f);
			glm::vec3 rayDir = glm::vec3(0.0f, -1.0f, 0.0f);

			float closestT = std::numeric_limits<float>::max();
			bool hit = false;

            for (auto terrainEntity : terrains) {
				auto& heightfield = terrains.get<HeightfieldColliderComponent>(terrainEntity);
                for (auto& tri : heightfield.worldTriangles) {
                    float t;
                    if (rayTriangleIntersect(rayOrigin, rayDir, tri.a, tri.b, tri.c, t)) {
                        if (t < closestT) {
							closestT = t;
							hit = true;
                        }
                    }
                }
            }

            if (!hit) return;
			//exact height of surafce bellow the object
			float terrainY = rayOrigin.y - closestT;

            //half height of palyer collider (for now assume it is just 1)
			const float playerHalfHeight = 1.0f;
			float minY = terrainY + playerHalfHeight;

            if (transform.position.y < minY) {
				transform.position.y = minY;
                if (rigidBody.velocity.y < 0.0f) {
					rigidBody.velocity.y = 0.0f;
                }
            }
        }
    );
}

void TerrainCollisionSystem::updateCamera(Camera& camera, entt::registry& registry) {
	auto terrains = registry.view<HeightfieldColliderComponent>();

	glm::vec3 rayOrigin = camera.Position + glm::vec3(0.0f, 3.0f, 0.0f);
	glm::vec3 rayDir = glm::vec3(0.0f, -1.0f, 0.0f);

	float closestT = std::numeric_limits<float>::max();
	bool hit = false;

    for (auto& terrainEntity : terrains) {
		auto& heightfield = terrains.get<HeightfieldColliderComponent>(terrainEntity);
        for (auto& tri : heightfield.worldTriangles) {
            float t;
                if (rayTriangleIntersect(rayOrigin, rayDir, tri.a, tri.b, tri.c, t)) {
                    if (t < closestT) {
						closestT = t;
						hit = true;
                    }
                }
        }
    }

    if (!hit) return;

	float terrainY = rayOrigin.y - closestT;

	//height of eyes of camera from the ground
	const float eyeHeight = 1.8f;
    if (camera.Position.y < terrainY + eyeHeight) {
		camera.Position.y = terrainY + eyeHeight;
    }
}