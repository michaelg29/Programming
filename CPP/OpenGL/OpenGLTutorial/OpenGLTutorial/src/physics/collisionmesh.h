#ifndef COLLISIONMESH_H
#define COLLISIONMESH_H

#include <vector>

#include <glm/glm.hpp>

#include "../algorithms/bounds.h"

// forward declaration
class CollisionModel;
class CollisionMesh;
class RigidBody;

typedef struct Face {
    CollisionMesh* mesh;
    unsigned int i1, i2, i3;

    glm::vec3 baseNormal;
    glm::vec3 norm;

    bool collidesWithSphere(RigidBody* thisRb, BoundingRegion br);
    bool collidesWith(RigidBody* thisRb, struct Face& face, RigidBody* faceRb);
} Face;

class CollisionMesh {
public:
    CollisionModel* model;
    BoundingRegion br;

    std::vector<glm::vec3> points;
    std::vector<Face> faces;

    CollisionMesh(unsigned int noPoints, float* coordinates, unsigned int noFaces, unsigned int* indices);
};

#endif