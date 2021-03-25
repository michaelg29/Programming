#include "rigidbody.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

// test for equivalence of two rigid bodies
bool RigidBody::operator==(RigidBody rb) {
    return instanceId == rb.instanceId;
}

// test for equivalence of two rigid bodies
bool RigidBody::operator==(std::string id) {
    return instanceId == id;
}

/*
    constructor
*/

// construct with parameters and default
RigidBody::RigidBody(std::string modelId, glm::vec3 size, float mass, glm::vec3 pos, glm::vec3 rot)
    : modelId(modelId), size(size), mass(mass), pos(pos), rot(rot), velocity(0.0f), acceleration(0.0f), state(0) {
    update(0.0f);
}

/*
    transformation functions
*/

// update position with velocity and acceleration
void RigidBody::update(float dt) {
    pos += velocity * dt + 0.5f * acceleration * (dt * dt);
    velocity += acceleration * dt;

    glm::mat4 rotMat = glm::toMat4(glm::quat(rot));
    //glm::mat4 rot(1.0f);

    // model = trans * rot * scale = TRS
    model = glm::translate(glm::mat4(1.0f), pos);
    model = model * rotMat;
    model = glm::scale(model, size);

    normalModel = glm::transpose(glm::inverse(glm::mat3(model)));
}

// apply a force
void RigidBody::applyForce(glm::vec3 force) {
    acceleration += force / mass;
}

// apply a force
void RigidBody::applyForce(glm::vec3 direction, float magnitude) {
    applyForce(direction * magnitude);
}

// apply an acceleration (remove redundancy of dividing by mass)
void RigidBody::applyAcceleration(glm::vec3 a) {
    acceleration += a;
}

// apply an acceleration (remove redundancy of dividing by mass)
void RigidBody::applyAcceleration(glm::vec3 direction, float magnitude) {
    applyAcceleration(direction * magnitude);
}

// apply force over time
void RigidBody::applyImpulse(glm::vec3 force, float dt) {
    velocity += force / mass * dt;
}

// apply force over time
void RigidBody::applyImpulse(glm::vec3 direction, float magnitude, float dt) {
    applyImpulse(direction * magnitude, dt);
}

// transfer potential or kinetic energy from another object
void RigidBody::transferEnergy(float joules, glm::vec3 direction) {
    if (joules == 0) {
        return;
    }

    // comes from formula: KE = 1/2 * m * v^2
    glm::vec3 deltaV = sqrt(2 * abs(joules) / mass) * direction;

    velocity += joules > 0 ? deltaV : -deltaV;
}