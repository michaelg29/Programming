#include "rigidbody.h"

#include "../io/keyboard.h"

#include <iostream>

std::string RigidBody::currentId = "aaaaaaaa";

std::string RigidBody::generateId() {
	for (int i = currentId.length() - 1; i >= 0; i--) {
		if ((int)currentId[i] != (int)'z') {
			currentId[i] = (char)(((int)currentId[i]) + 1);
			break;
		}
		else {
			currentId[i] = 'a';
		}
	}

	return currentId;
}

RigidBody::RigidBody(std::string* modelId, glm::vec3 size, float mass, glm::vec3 pos, glm::vec3 velocity, glm::vec3 acceleration)
	: modelId(modelId), size(size), mass(mass), pos(pos), velocity(velocity), acceleration(acceleration) {}

void RigidBody::update(float dt) {
	pos += velocity * dt + 0.5f * acceleration * (dt * dt);
	velocity += acceleration * dt;
}

void RigidBody::applyForce(glm::vec3 force) {
	acceleration += force / mass;
}

void RigidBody::applyForce(glm::vec3 direction, float magnitude) {
	applyForce(direction * magnitude);
}

void RigidBody::applyImpulse(glm::vec3 force, float dt) {
	velocity += force / mass * dt;
}

void RigidBody::applyImpulse(glm::vec3 direction, float magnitude, float dt) {
	applyImpulse(direction * magnitude, dt);
}

void RigidBody::applyAcceleration(glm::vec3 a) {
	acceleration += a;
}

void RigidBody::removeForce(glm::vec3 force) {
	acceleration -= force / mass;
}

void RigidBody::removeAcceleration(glm::vec3 a) {
	acceleration -= a;
}

void RigidBody::transferEnergy(float joules, glm::vec3 direction) {
	if (joules == 0) {
		return;
	}

	// comes from equation KE = 1/2*m*v^2
	glm::vec3 deltaV = (sqrt(2 * abs(joules) / mass)) * direction;
	
	velocity += joules > 0 ? deltaV : -deltaV;
}

bool RigidBody::operator==(RigidBody rb) {
	return instanceId == rb.instanceId;
}

bool RigidBody::operator==(std::string id) {
	return instanceId == id;
}