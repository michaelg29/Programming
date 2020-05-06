#include "../model.h"

#include "../../io/camera.h"

glm::quat rotationBetweenVectors(glm::vec3 start, glm::vec3 end);

class Gun : public Model {
public:
	Gun()
		: Model(glm::vec3(8.0f, -3.0f, 0.0f), glm::vec3(0.05f), true) {}

	void init() {
		loadModel("assets/models/m4a1/scene.gltf");
	}

	void render(Shader shader) {
		glm::mat4 model = glm::mat4(1.0f);

		// translate forward and down
		glm::vec3 down = glm::cross(Camera::defaultCamera.cameraFront, Camera::defaultCamera.cameraRight);
		pos = Camera::defaultCamera.cameraPos + glm::vec3(Camera::defaultCamera.cameraFront * 8.0f) + glm::vec3(down * 3.14f);
		model = glm::translate(model, pos);

		// rotate around cameraRight with dot product of cameraFront and cameraRight
		float theta = acos(glm::dot(Camera::defaultCamera.cameraUp, Camera::defaultCamera.cameraFront) /
			glm::length(Camera::defaultCamera.cameraUp) / glm::length(Camera::defaultCamera.cameraFront));
		model = glm::rotate(model, Model::PI / 2 - theta, Camera::defaultCamera.cameraRight); // subtract pi/2 because angle btwn camUp and gunFront

		// rotate around worldUp with dot product of cameraFront and initial gun position
		glm::vec2 front2d = glm::vec2(Camera::defaultCamera.cameraFront.x, Camera::defaultCamera.cameraFront.z);
		theta = acos(glm::dot(glm::vec2(1.0f, 0.0f), front2d) / glm::length(front2d));
		model = glm::rotate(model, front2d.y < 0 ? theta : -theta, Camera::defaultCamera.cameraUp);

		// scale
		model = glm::scale(model, size);

		shader.setMat4("model", model);

		Model::render(shader, false);
	}
};

glm::quat rotationBetweenVectors(glm::vec3 start, glm::vec3 dest) {
	start = glm::normalize(start);
	dest = glm::normalize(dest);

	float cosTheta = glm::dot(start, dest);
	glm::vec3 rotationAxis;

	if (cosTheta < -1 + 0.001f) {
		// special case when vectors in opposite directions:
		// there is no "ideal" rotation axis
		// So guess one; any will do as long as it's perpendicular to start
		rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
		if (pow(glm::length(rotationAxis), 2) < 0.01) // bad luck, they were parallel, try again!
			rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start);

		rotationAxis = normalize(rotationAxis);
		return glm::quat(glm::radians(180.0f), rotationAxis);
	}

	rotationAxis = cross(start, dest);

	float s = sqrt((1 + cosTheta) * 2);
	float invs = 1 / s;

	return glm::quat(
		s * 0.5f,
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);
}