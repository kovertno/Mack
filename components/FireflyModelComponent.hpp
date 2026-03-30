#ifndef FIREFLY_MODEL_COMPONENT_H
#define FIREFLY_MODEL_COMPONENT_H

#include <vector>
#include <glm/glm.hpp>

struct FireflyModelComponent {
	glm::vec3 startingPos{};
	float offset{};
};

#endif