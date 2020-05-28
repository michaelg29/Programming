#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <glm/glm.hpp>

#include <assimp/scene.h>

#include "shader.h"
#include "texture.h"

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
	BoundingRegion br;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	aiColor4D diff;
	aiColor4D spec;

	unsigned int VAO;

	Mesh(BoundingRegion br, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures = {});
	Mesh(BoundingRegion br, std::vector<Vertex> vertices, std::vector<unsigned int> indices, aiColor4D diff, aiColor4D spec);

	void render(Shader shader, glm::vec3 pos, glm::vec3 size, Box *b, bool doRender = true);

	void cleanup();

private:
	bool noTex;

	unsigned int VBO, EBO;

	void setup();
};

#endif