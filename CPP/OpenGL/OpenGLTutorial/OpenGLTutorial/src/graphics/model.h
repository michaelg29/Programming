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

#include "models/box.hpp"

#include "../physics/rigidbody.h"

#include "../algorithms/bounds.h"

class Model {
public:
	glm::vec3 size;
	std::vector<Mesh> meshes;

	BoundTypes boundType;

	RigidBody rb;

	Model(BoundTypes boundType = BoundTypes::AABB, glm::vec3 pos = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f), bool noTex = false, bool dynamic = false);

	void init();

	void loadModel(std::string path);

	void render(Shader shader, float dt, Box *b, bool setModel = true, bool doRender = true);
	
	void cleanup();

protected:
	bool noTex;
	bool dynamic;

	// model data
	
	std::string directory;

	std::vector<Texture> textures_loaded;

	// functions
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type);
};

#endif