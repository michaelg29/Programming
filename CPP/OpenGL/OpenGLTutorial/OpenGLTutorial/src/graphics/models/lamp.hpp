#ifndef LAMP_HPP
#define LAMP_HPP

#include "cube.hpp"
#include "../shader.h"
#include "../light.h"

class Lamp : public Cube {
public:
	glm::vec3 lightColor;

	PointLight light;

	Lamp(glm::vec3 lightColor = glm::vec3(1.0f),
		glm::vec3 ambient = glm::vec3(1.0f),
		glm::vec3 diffuse = glm::vec3(1.0f),
		glm::vec3 specular = glm::vec3(1.0f),
		float k0 = 1.0f,
		float k1 = 0.09f,
		float k2 = 0.032f,
		glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f))
		: lightColor(lightColor),
		light({ pos, k0, k1, k2, glm::vec4(ambient, 1.0f), glm::vec4(diffuse, 1.0f), glm::vec4(specular, 1.0f) }),
		Cube(pos, size) {}

	void render(Shader shader) {
		shader.set3Float("lightColor", lightColor);

		Cube::render(shader);
	}
};

#endif