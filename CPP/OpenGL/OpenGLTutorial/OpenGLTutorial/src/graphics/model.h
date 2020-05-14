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

#include "../physics/rigidbody.h"

class Model {
public:
	glm::vec3 size;

	RigidBody rb;

	Model(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f), bool noTex = false, bool dynamic = false);

	void init();

	void loadModel(std::string path);

	void render(Shader shader, float dt, bool setModel = true);
	
	void cleanup();

protected:
	bool noTex;
	bool dynamic;

	// model data
	std::vector<Mesh> meshes;
	std::string directory;

	std::vector<Texture> textures_loaded;

	// functions
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type);
};

template <class T>
class ModelArray {
public:
	std::vector<RigidBody> instances;

	void init() {
		model.init();
	}

	void render(Shader shader, float dt) {
		for (RigidBody rb : instances) {
			rb.update(dt);
			model.rb.pos = rb.pos;
			model.render(shader, dt);
		}
	}

	void setSize(glm::vec3 size) {
		model.size = size;
	}

	void cleanup() {
		model.cleanup();
	}

protected:
	T model;
};

#endif