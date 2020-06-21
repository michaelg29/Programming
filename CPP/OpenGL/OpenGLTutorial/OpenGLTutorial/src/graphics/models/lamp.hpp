#ifndef LAMP_HPP
#define LAMP_HPP

#include "cube.hpp"
#include "modelArray.hpp"
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

//class LampArray : public ModelArray<Lamp> {
//public:
//	std::vector<PointLight> lightInstances;
//
//	void init() {
//		model = Lamp(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.05f), glm::vec3(0.8f), glm::vec3(1.0f),
//			1.0f, 0.09f, 0.032f,
//			glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.25f));
//		ModelArray::init();
//	}
//
//	void render(Shader shader, float dt, Box *b) {
//		positions.clear();
//		sizes.clear();
//
//		for (PointLight& pl : lightInstances) {
//			positions.push_back(pl.position);
//			sizes.push_back(model.size);
//		}
//
//		ModelArray::render(shader, dt, b, false);
//	}
//};

#endif