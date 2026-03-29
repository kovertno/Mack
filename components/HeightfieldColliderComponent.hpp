#pragma once
#include <vector>
#include <glm/glm.hpp>

struct Triangle {
	glm::vec3 a, b, c;
};

struct HeightfieldColliderComponent {
	std::vector<Triangle> worldTriangles;
	bool baked = false;
};