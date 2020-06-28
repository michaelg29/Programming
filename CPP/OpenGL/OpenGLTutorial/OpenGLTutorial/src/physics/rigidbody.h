#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <glm/glm.hpp>
#include <string>

#define DEAD	(unsigned char)0b00000001
#define MOVED	(unsigned char)0b00000010

class RigidBody {
public:
	unsigned char state;

	float mass;

	glm::vec3 pos;
	glm::vec3 velocity;
	glm::vec3 acceleration;

	glm::vec3 size;

	std::string modelId;
	std::string instanceId;

	RigidBody() {}
	RigidBody(std::string modelId, glm::vec3 size = glm::vec3(1.0f), float mass = 1.0f, glm::vec3 pos = glm::vec3(0.0f));

	void update(float dt);

	void applyForce(glm::vec3 force);
	void applyForce(glm::vec3 direction, float magniutde);
	void applyImpulse(glm::vec3 force, float dt);
	void applyImpulse(glm::vec3 force, float magnitude, float dt);

	void applyAcceleration(glm::vec3 acceleration);

	void removeForce(glm::vec3 force);
	void removeAcceleration(glm::vec3 acceleration);

	void transferEnergy(float joules, glm::vec3 direction);

	bool operator==(RigidBody rb);
	bool operator==(std::string id);
};

#endif