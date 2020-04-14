#ifndef LAMP_HPP
#define LAMP_HPP

#include "cube.hpp"
#include "../material.h"
#include "../shader.h"

class Lamp : public Cube {
public:
	glm::vec3 lightColor;

	// light strength values
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	// attenuation constants
	float k0;
	float k1;
	float k2;

	Lamp(glm::vec3 lightColor,
		glm::vec3 ambient = glm::vec3(1.0f),
		glm::vec3 diffuse = glm::vec3(1.0f),
		glm::vec3 specular = glm::vec3(1.0f),
		float k0 = 1.0f,
		float k1 = 0.09f,
		float k2 = 0.032f,
		glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f))
		: lightColor(lightColor),
		ambient(ambient),
		diffuse(diffuse),
		specular(specular),
		k0(k0), k1(k1), k2(k2),
		Cube(pos, size) {}

	void render(Shader shader) {
		shader.set3Float("lightColor", lightColor);

		//lightColor.x = sin(glfwGetTime() * 2.0f);
		//lightColor.y = sin(glfwGetTime() * 0.7f);
		//lightColor.z = sin(glfwGetTime() * 1.3f);

		//diffuse = lightColor * glm::vec3(0.5f);
		//ambient = diffuse * glm::vec3(0.2f);

		Cube::render(shader);
	}
};

#endif