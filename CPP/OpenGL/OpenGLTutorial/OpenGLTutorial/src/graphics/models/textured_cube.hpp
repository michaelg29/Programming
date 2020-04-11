#ifndef TEXTURED_CUBE_HPP
#define TEXTURED_CUBE_HPP

#include "cube.hpp"
#include "../texture.h"
#include "../material.h"
#include <vector>

class TexturedCube : public Cube {
public:
	std::vector<Texture> textures;

	TexturedCube(Material material, std::vector<Texture> textures, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f))
		: textures(textures), Cube(material, pos, size) {}

	void init() {
		Cube::init();

		for (Texture t : textures) {
			mesh.textures.push_back(t);
		}
	}
};

#endif