#include "../model.h"

#include "../../io/camera.h"

class Gun : public Model {
public:
	Gun()
		: Model(BoundTypes::AABB, glm::vec3(0.0f), glm::vec3(1 / 300.0f), true) {
		rb.mass = 1000;
	}

	void init() {
		loadModel("assets/models/m4a1/scene.gltf");
	}

	void render(Shader shader, double dt, Camera cam, Box *b) {
		glm::mat4 model = glm::mat4(1.0f);

		// translate forward and down
		glm::vec3 down = glm::cross(cam.cameraFront, cam.cameraRight);
		rb.pos = cam.cameraPos + glm::vec3(cam.cameraFront * 0.5f) + glm::vec3(down * 0.205f);
		model = glm::translate(model, rb.pos);

		// rotate around cameraRight with dot product of cameraFront and cameraRight
		float theta = acos(glm::dot(cam.cameraUp, cam.cameraFront) /
			glm::length(cam.cameraUp) / glm::length(cam.cameraFront));
		model = glm::rotate(model, atanf(1) * 2 - theta, cam.cameraRight); // subtract pi/2 because angle btwn camUp and gunFront

		// rotate around worldUp with dot product of cameraFront and initial gun position
		glm::vec2 front2d = glm::vec2(cam.cameraFront.x, cam.cameraFront.z);
		theta = acos(glm::dot(glm::vec2(1.0f, 0.0f), front2d) / glm::length(front2d));
		model = glm::rotate(model, front2d.y < 0 ? theta : -theta, cam.cameraUp);

		// scale
		model = glm::scale(model, size);

		shader.setMat4("model", model);

		Model::render(shader, dt, b, false);
	}

	void launch() {

	}
};