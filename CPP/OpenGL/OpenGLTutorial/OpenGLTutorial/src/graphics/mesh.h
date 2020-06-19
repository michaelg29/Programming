#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <glm/glm.hpp>

#include <assimp/scene.h>

#include "shader.h"
#include "texture.h"
#include "glmemory.hpp"

#include "models/box.hpp"

#include "../algorithms/bounds.h"

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texCoord;

	static std::vector<Vertex> genList(float* vertices, int noVertices);
};

class Mesh {
public:
	// bounding region enclosing mesh
	BoundingRegion br;

	// model lists
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	
	// textures
	std::vector<Texture> textures;

	// material values
	aiColor4D diff;
	aiColor4D spec;

	// VAO for entire mesh
	ArrayObject VAO;

	// default constructor 
	Mesh();

	// initialize as textured object
	Mesh(BoundingRegion br, std::vector<Texture> textures = {});

	// initialize as material object
	Mesh(BoundingRegion br, aiColor4D diff, aiColor4D spec);

	// load vertex and index data
	void loadData(Vertex* vertices, unsigned int noVertices, unsigned int* indices, unsigned int noIndices);

	// render number of instances
	void render(Shader shader, unsigned int noInstances);

	// cleanup (free vectors, cleanup VAO)
	void cleanup();

private:
	bool noTex;
};

#endif