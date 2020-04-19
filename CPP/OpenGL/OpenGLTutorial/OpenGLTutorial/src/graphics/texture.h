#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>

#include <assimp/scene.h>

#include <string>

class Texture {
public:
	Texture();
	Texture(std::string dir, std::string path, aiTextureType type);

	void generate();
	void load(bool flip = true);

	void bind();

	// texture object
	unsigned int id;
	aiTextureType type;
	std::string dir;
	std::string path;

protected:
	// img properties
	int width;
	int height;
	int nChannels;
};

#endif