#ifndef MODEL_MESH_H
#define MODEL_MESH_H

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.hpp"

#include <string>
#include <vector>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id {};
	std::string type;
	std::string path; // we store the path of the texture to compare with other textures
};


class ModelMesh {
private:
	unsigned int VAO, VBO, EBO;

	void SetupMesh();
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	glm::vec3 baseColor;

	ModelMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, glm::vec3 baseColor);
	~ModelMesh() = default;

	void Draw(Shader* shader);
};

#endif