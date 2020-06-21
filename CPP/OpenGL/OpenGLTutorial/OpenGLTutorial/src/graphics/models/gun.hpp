#include "../model.h"

#include "../../io/camera.h"

class Gun : public Model {
public:
	Gun(unsigned int maxNoInstances)
		: Model("m4a1", BoundTypes::AABB, maxNoInstances, CONST_INSTANCES | NO_TEX) {}

	void init() {
		loadModel("assets/models/m4a1/scene.gltf");
	}

	void render(Shader shader, float dt, Scene* scene, bool setModel = true) {
		glm::mat4 model = glm::mat4(1.0f);

		// translate forward and down
		glm::vec3 down = glm::cross(scene->getActiveCamera()->cameraFront, scene->getActiveCamera()->cameraRight);
		instances[0].pos = scene->getActiveCamera()->cameraPos + glm::vec3(scene->getActiveCamera()->cameraFront * 0.5f) + glm::vec3(down * 0.205f);
		model = glm::translate(model, instances[0].pos);

		// rotate around cameraRight with dot product of cameraFront and cameraRight
		float theta = acos(glm::dot(scene->getActiveCamera()->cameraUp, scene->getActiveCamera()->cameraFront) /
			glm::length(scene->getActiveCamera()->cameraUp) / glm::length(scene->getActiveCamera()->cameraFront));
		model = glm::rotate(model, atanf(1) * 2 - theta, scene->getActiveCamera()->cameraRight); // subtract pi/2 because angle btwn camUp and gunFront

		// rotate around worldUp with dot product of cameraFront and initial gun position
		glm::vec2 front2d = glm::vec2(scene->getActiveCamera()->cameraFront.x, scene->getActiveCamera()->cameraFront.z);
		theta = acos(glm::dot(glm::vec2(1.0f, 0.0f), front2d) / glm::length(front2d));
		model = glm::rotate(model, front2d.y < 0 ? theta : -theta, scene->getActiveCamera()->cameraUp);

		// scale
		model = glm::scale(model, instances[0].size);

		shader.setMat4("model", model);

		Model::render(shader, dt, scene, false);
	}
};