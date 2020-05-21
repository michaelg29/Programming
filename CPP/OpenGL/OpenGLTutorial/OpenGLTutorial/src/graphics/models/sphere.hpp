#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "../model.h"
#include "modelArray.hpp"

class Sphere : public Model {
public:
	Sphere(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f), bool dynamic = false)
		: Model(pos, size, true, dynamic) {
	}

	void init() {
		loadModel("assets/models/sphere/scene.gltf");
	}
};

class SphereArray : public ModelArray<Sphere> {
public:
	void init() {
		model = Sphere(glm::vec3(0.0f), glm::vec3(0.05f));
		ModelArray::init();
	}
};

#endif