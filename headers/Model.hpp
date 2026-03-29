#ifndef MODEL_H
#define MODEL_H

#include "Shader.hpp"
#include "ModelMesh.hpp"

#include "stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <string>
#include <set>

class Model {
private:
	std::vector<Texture> texturesLoaded; // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once, since texture loading takes a lot of processing power.
	std::vector<ModelMesh> meshes;
	std::string directory;

	// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void ProcessNode(aiNode* node, const aiScene* scene);
	ModelMesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	// checks all material textures of a given type and loads the textures if they're not loaded yet.
	// the required info is returned as a Texture struct.
	std::vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string& typeName);

public:
	Model() = default;
	~Model() = default;

	void Draw(Shader* shader);

	// loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void LoadModel(const std::string& path);

	//retrive all the vertex data
	const std::vector<ModelMesh>& GetMeshes() const {
		return meshes;
	}
};

#endif