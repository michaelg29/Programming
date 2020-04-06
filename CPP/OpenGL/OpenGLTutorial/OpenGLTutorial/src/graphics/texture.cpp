#include "texture.h"

#include <iostream>

int Texture::currentId = 0;

Texture::Texture() 
	: path(""), name(""), id(-1), width(0), height(0), nChannels(0) {}

Texture::Texture(const char* path, const char* name, bool defaultParams)
	: path(path), name(name), id(currentId++),
		width(0), height(0), nChannels(0)
{
	generate();

	if (defaultParams) {
		setFilters(GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
		setWrap(GL_REPEAT);
	}
}

void Texture::generate() {
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
}

void Texture::load(bool flip) {
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(path, &width, &height, &nChannels, 0);
	
	GLenum colorMode;
	if (nChannels == 1) {
		colorMode = GL_RED;
	}
	else if (nChannels == 3) {
		colorMode = GL_RGB;
	}
	else if (nChannels == 4) {
		colorMode = GL_RGBA;
	}

	if (data) {
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, colorMode, width, height, 0, colorMode, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture at " << path << std::endl;
	}

	stbi_image_free(data);
}

void Texture::setFilters(GLint all) {
	setFilters(all, all);
}

void Texture::setFilters(GLint mag, GLint min) {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
}

void Texture::setWrap(GLint all) {
	setWrap(all, all);
}

void Texture::setWrap(GLint s, GLint t) {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
}

void Texture::activate() {
	glActiveTexture(GL_TEXTURE0 + id);
}

void Texture::setBorderColor(float borderColor[4]) {
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}