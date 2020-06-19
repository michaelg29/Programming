#ifndef TROLL_HPP
#define TROLL_HPP

#include "../model.h"
#include "modelArray.hpp"

class Troll : public Model {
public:
	Troll(unsigned int maxNoInstances)
		: Model("troll", BoundTypes::AABB, maxNoInstances, CONST_INSTANCES) {}

	void init() {
		loadModel("assets/models/lotr_troll/scene.gltf");
	}
};

#endif