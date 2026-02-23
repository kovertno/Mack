#ifndef CROSSHAIR_MESH_COMPONENT_H
#define CROSSHAIR_MESH_COMPONENT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct CrosshairMeshComponent {
	unsigned int numOfVertices;
	unsigned int VAO;

	static void SetCrosshairVAO(unsigned int& VAO, unsigned int& VBO) {
		float vertices[] = {
			-0.5f, 0.5f,
			-0.5f,-0.5f,
			 0.5f,-0.5f,
			 0.5f,-0.5f,
			-0.5f, 0.5f,
			 0.5f, 0.5f
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