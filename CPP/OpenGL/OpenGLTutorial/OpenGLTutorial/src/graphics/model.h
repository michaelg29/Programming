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
#include "glmemory.hpp"

#include "models/box.hpp"

#include "../physics/rigidbody.h"

#include "../algorithms/bounds.h"
#include "../algorithms/states.hpp"

// model switches
#define DYNAMIC			(unsigned char)0b00000001
#define CONST_INSTANCES (unsigned char)0b00000010
#define NO_TEX			(unsigned char)0b00000100

class Scene; // forward declaration

class Model {
public:
	// list of meshes
	std::vector<Mesh> meshes;
	// list of bounding regions
	std::vector<BoundingRegion> boundingRegions;

	// type of bounding region for meshes
	BoundTypes boundType;

	// model id
	std::string id;

	// maximum number of allowed instances
	int maxNoInstances;
	// current number of occupied instances
	int currentNoInstances;

	// list of instances
	std::vector<RigidBody*> instances;

	// combination of switches in ModelStates
	unsigned char switches;

	// constructor
	Model(std::string id, BoundTypes boundType, int maxNoInstances, unsigned char flags = 0);

	// initialize method (to be overriden in subclasses)
	virtual void init();

	// generate instance of model
	RigidBody* generateInstance(glm::vec3 size, float mass, glm::vec3 pos);
	
	// initialize VBO memory for instances (positions/sizes)
	void initInstances();

	// load model from object file
	void loadModel(std::string path);

	// remove instance using instance id
	void removeInstance(std::string instanceId);

	// remove instance using instance idx
	void removeInstance(int idx);

	// render model with shader (may be overriden)
	virtual void render(Shader shader, float dt, Scene *scene, bool setModel = true);
	
	// cleanup model (all meshes)
	void cleanup();

	// get index of instance with id
	unsigned int getIdx(std::string id);

protected:
	// model data
	
	// directory containing model file
	std::string directory;

	// list of textures that have been loaded
	std::vector<Texture> textures_loaded;

	// model loading function
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type);

	// VBO's for positions and sizes
	BufferObject posVBO;
	BufferObject sizeVBO;
};

#endif