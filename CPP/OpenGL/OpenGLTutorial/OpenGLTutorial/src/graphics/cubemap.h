#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>

#include <string>
#include <vector>

#include "shader.h"
#include "vertexmemory.hpp"

class Scene;

class Cubemap {
public:
	Cubemap();

	void allocate(GLenum format, GLuint width, GLuint height, GLenum type);

	void loadTextures(std::string dir,
		std::string right	= "right.png",
		std::string left	= "left.png",
		std::string top		= "top.png",
		std::string bottom	= "bottom.png",
		std::string front	= "front.png",
		std::string back	= "back.png");

	void init();

	void bind();

	void render(Shader shader, Scene* scene);

	GLuint getId();

	void cleanup();

private:
	// texture object
	GLuint id;
	std::string dir;
	std::vector<std::string> faces;
	bool hasTextures;

	// cube object
	ArrayObject VAO;
};

#endif