#ifndef LAMP_HPP
#define LAMP_HPP

#include "cube.hpp"
#include "../shader.h"
#include "../light.h"

class Lamp : public Cube {
public:
	glm::vec3 lightColor;

	Lamp(unsigned int maxNoInstances, glm::vec3 lightColor = glm::vec3(1.0f))
		: Cube(maxNoInstances) {
		id = "lamp";
		this->lightColor = lightColor;
	}

	void render(Shader shader, float dt, Scene* scene, bool setModel = true) {
		shader.set3Float("lightColor", lightColor);

		Cube::render(shader, dt, scene, setModel);
	}
};

#endif