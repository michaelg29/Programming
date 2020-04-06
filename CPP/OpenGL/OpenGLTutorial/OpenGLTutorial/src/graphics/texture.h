#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>

class Texture {
public:
	Texture();
	Texture(const char* path, const char* name, bool defaultParams = true);

	void generate();
	void load(bool flip = true);

	void setFilters(GLint all);
	void setFilters(GLint mag, GLint min);

	void setWrap(GLint all);
	void setWrap(GLint s, GLint t);

	void setBorderColor(float borderColor[4]);

	void activate();

	// texture object
	int id;
	unsigned int tex;
	const char* name;

protected:
	static int currentId;

	// img properties
	const char* path;
	int width;
	int height;
	int nChannels;
};

#endif