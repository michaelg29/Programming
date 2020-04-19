#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

#include "shader.h"
#include "texture.h"
#include "mesh.h"

class Model {
public:
	glm::vec3 pos;
	glm::vec3 size;

	Model(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f));

	void loadModel(std::string path);

	void render(Shader shader);
	
	void cleanup();

protected:
	// model data
	std::vector<Mesh> meshes;
	std::string directory;

	std::vector<Texture> textures_loaded;

	// functions
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type);
};

#endif