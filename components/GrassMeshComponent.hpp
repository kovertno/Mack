#ifndef GRASS_MESH_COMPONENT
#define GRASS_MESH_COMPONENT

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct GrassMeshComponent {
	unsigned int numOfVertices;
	unsigned int VAO;

	static void SetGrassVAO(unsigned int& VAO, unsigned int& VBO) {
		float vertices[] = {
			-0.3f, -1.0f,
			 0.3f, -1.0f,
			 0.0f,	1.0f
		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}
};

#endif