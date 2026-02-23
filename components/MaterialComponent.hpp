#ifndef MATERIAL_COMPONENT_H
#define MATERIAL_COMPONENT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct MaterialComponent {
	glm::vec3 ambient{ 0.3f };
	glm::vec3 diffuse{ 0.7f };
	glm::vec3 specular{ 1.0f };
	float shininess = 32.0f;
};

#endif
