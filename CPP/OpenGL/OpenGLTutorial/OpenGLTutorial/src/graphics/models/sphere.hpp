#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "../model.h"
#include "modelArray.hpp"

class Sphere : public Model {
public:
	Sphere(unsigned int maxNoInstances)
		: Model("sphere", BoundTypes::SPHERE, maxNoInstances, NO_TEX | DYNAMIC) {}

	void init() {
		loadModel("assets/models/sphere/scene.gltf");
	}
};

//class SphereArray : public ModelArray<Sphere> {
//public:
//	void init() {
//		model = Sphere(glm::vec3(0.0f), glm::vec3(0.25f));
//		ModelArray::init();
//	}
//};

#endif