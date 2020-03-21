#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float _yaw, float _pitch)
	: cameraPos(position),
	worldUp(up),
	yaw(_yaw),
	pitch(_pitch),
	speed(2.5f),
	sensitivity(0.1f),
	zoom(45.0f),
	cameraFront(glm::vec3(0.0f, 0.0f, -1.0f))
	{
	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float _yaw, float _pitch)
	: Camera(glm::vec3(posX, posY, posZ), glm::vec3(upX, upY, upZ), _yaw, _pitch)
	{
	updateCameraVectors();
}


void Camera::updateCameraDirection(double dx, double dy) {
	yaw += dx; 
	pitch += dy;

	if (pitch > 89.0f) {
		pitch = 89.0f;
	}
	if (pitch < -89.0f) {
		pitch = -89.0f;
	}

	updateCameraVectors();
}

void Camera::updateCameraPos(CameraMovement direction, double dt) {
	float velocity = (float)dt * speed;

	switch (direction) {
	case CameraMovement::FORWARD:
		cameraPos += cameraFront * velocity;
		break;
	case CameraMovement::BACKWARD:
		cameraPos -= cameraFront * velocity;
		break;
	case CameraMovement::RIGHT:
		cameraPos += cameraRight * velocity;
		break;
	case CameraMovement::LEFT:
		cameraPos -= cameraRight * velocity;
		break;
	case CameraMovement::UP:
		cameraPos += cameraUp * velocity;
		break;
	case CameraMovement::DOWN:
		cameraPos -= cameraUp * velocity;
		break;
	}
}

void Camera::updateCameraZoom(double dy) {
	if (zoom >= 1.0f && zoom <= 45.0f) {
		zoom -= dy;
	}
	else if (zoom < 1.0f) {
		zoom = 1.0f;
	}
	else {
		zoom = 45.0f;
	}
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Camera::updateCameraVectors() {
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);

	cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}