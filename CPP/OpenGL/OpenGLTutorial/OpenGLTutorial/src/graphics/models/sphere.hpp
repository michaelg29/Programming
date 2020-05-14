#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "../model.h"

class Sphere : public Model {
public:
	Sphere(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f), bool dynamic = false)
		: Model(pos, size, true, dynamic) {
	}

	void init() {
		loadModel("assets/models/sphere/scene.gltf");
	}

	void render(Shader shader, float dt, bool setModel = true) {
		Model::render(shader, dt, setModel);
	}
};

class SphereArray : public ModelArray<Sphere> {
public:
	void init() {
		model = Sphere(glm::vec3(0.0f), glm::vec3(0.25f));
		model.init();
	}

	void render(Shader shader, float dt) {
 		for (RigidBody &rb : instances) {
			rb.update(dt);
			model.rb.pos = rb.pos;
			model.render(shader, dt);
		}
	}
};

#endif