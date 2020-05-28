#ifndef TROLL_HPP
#define TROLL_HPP

#include "../model.h"
#include "modelArray.hpp"

class Troll : public Model {
public:
	Troll(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 size = glm::vec3(0.05f), bool dynamic = false)
		: Model(BoundTypes::AABB, pos, size, false, dynamic) {
	}

	void init() {
		loadModel("assets/models/lotr_troll/scene.gltf");
	}
};

#endif