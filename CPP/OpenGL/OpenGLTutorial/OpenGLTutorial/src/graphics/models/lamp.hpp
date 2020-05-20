#ifndef LAMP_HPP
#define LAMP_HPP

#include "cube.hpp"
#include "modelArray.hpp"
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

	void render(Shader shader, double dt, bool setModel = true, bool doRender = true) {
		shader.set3Float("lightColor", lightColor);

		Cube::render(shader, dt, setModel, doRender);
	}
};

class LampArray : public ModelArray<Lamp> {
public:
	std::vector<PointLight> lightInstances;

	void init() {
		model = Lamp(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.05f), glm::vec3(0.8f), glm::vec3(1.0f),
			1.0f, 0.09f, 0.032f,
			glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.25f));
		ModelArray::init();
	}

	void render(Shader shader, float dt) {
		/*for (PointLight pl : lightInstances) {
			model.rb.pos = pl.position;

			model.render(shader, dt);
		}*/

		positions.clear();
		sizes.clear();

		for (PointLight& pl : lightInstances) {
			positions.push_back(pl.position);
			sizes.push_back(model.size);
		}

		ModelArray::render(shader, dt, false);
	}
};

#endif