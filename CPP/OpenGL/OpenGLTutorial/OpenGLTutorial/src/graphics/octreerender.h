#ifndef OCTREERENDER_H
#define OCTREERENDER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "octree.h"

class OctreeRender {
public:
	void setup();

	void render(Shader geoShader, Octree::node* octree);

	void cleanup();

private:
	unsigned int VAO, VBO;
};

#endif