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

	Lamp(glm::vec3 lightColor, 
		glm::vec3 ambient = glm::vec3(1.0f), 
		glm::vec3 diffuse = glm::vec3(1.0f), 
		glm::vec3 specular = glm::vec3(1.0f), 
		glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), 
		glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f))
		: lightColor(lightColor),
		ambient(ambient),
		diffuse(diffuse),
		specular(specular),
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