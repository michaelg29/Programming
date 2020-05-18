#include "../model.h"

#include "../../io/camera.h"

class Gun : public Model {
public:
	Gun()
		: Model(glm::vec3(0.0f), glm::vec3(1 / 300.0f), true) {
		rb.mass = 1000;
	}

	void init() {
		loadModel("assets/models/m4a1/scene.gltf");
	}

	void render(Shader shader, double dt) {
		glm::mat4 model = glm::mat4(1.0f);

		// translate forward and down
		glm::vec3 down = glm::cross(Camera::defaultCamera.cameraFront, Camera::defaultCamera.cameraRight);
		rb.pos = Camera::defaultCamera.cameraPos + glm::vec3(Camera::defaultCamera.cameraFront * 0.5f) + glm::vec3(down * 0.205f);
		model = glm::translate(model, rb.pos);

		// rotate around cameraRight with dot product of cameraFront and cameraRight
		float theta = acos(glm::dot(Camera::defaultCamera.cameraUp, Camera::defaultCamera.cameraFront) /
			glm::length(Camera::defaultCamera.cameraUp) / glm::length(Camera::defaultCamera.cameraFront));
		model = glm::rotate(model, atanf(1) * 2 - theta, Camera::defaultCamera.cameraRight); // subtract pi/2 because angle btwn camUp and gunFront

		// rotate around worldUp with dot product of cameraFront and initial gun position
		glm::vec2 front2d = glm::vec2(Camera::defaultCamera.cameraFront.x, Camera::defaultCamera.cameraFront.z);
		theta = acos(glm::dot(glm::vec2(1.0f, 0.0f), front2d) / glm::length(front2d));
		model = glm::rotate(model, front2d.y < 0 ? theta : -theta, Camera::defaultCamera.cameraUp);

		// scale
		model = glm::scale(model, size);

		shader.setMat4("model", model);

		Model::render(shader, dt, false);
	}

	void launch() {

	}
};