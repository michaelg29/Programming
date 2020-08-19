#ifndef TEXT_H
#define TEXT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>

#include <string>
#include <map>

#include "glmemory.hpp"
#include "shader.h"

// based off of https://learnopengl.com/img/in-practice/glyph.png
typedef struct {
	unsigned int	textureId;	// texture id storing character
	glm::ivec2		size;		// size of char
	glm::ivec2		bearing;	// distance from origin to top left of char
	unsigned int	advance;	// distance from origin to next origin (in 1/64 pixels)
} Character;

class TextRenderer {
public:
	TextRenderer();
	TextRenderer(int height);

	bool loadFont(FT_Library &ft, std::string path);

	void render(Shader shader, std::string text, float xPos, float yPos, float scale, glm::vec3 color);

private:
	int height;
	FT_Face fontFace;

	std::map<char, Character> chars;

	//ArrayObject 
	ArrayObject VAO;
};

#endif