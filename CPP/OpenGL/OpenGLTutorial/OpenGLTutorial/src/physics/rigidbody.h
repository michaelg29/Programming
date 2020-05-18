#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <glm/glm.hpp>

class RigidBody {
public:
	float mass;

	glm::vec3 pos;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	RigidBody(float mass = 1.0f, glm::vec3 pos = glm::vec3(0.0f), glm::vec3 velocity = glm::vec3(0.0f), glm::vec3 acceleration = glm::vec3(0.0f));

	void update(float dt);

	void applyForce(glm::vec3 force);
	void applyForce(glm::vec3 direction, float magniutde);
	void applyImpulse(glm::vec3 force, float dt);
	void applyImpulse(glm::vec3 force, float magnitude, float dt);

	void applyAcceleration(glm::vec3 acceleration);

	void removeForce(glm::vec3 force);
	void removeAcceleration(glm::vec3 acceleration);

	void transferEnergy(float joules);
};

#endif