#ifndef COLLISIONMESH_H
#define COLLISIONMESH_H

#include <vector>

#include <glm/glm.hpp>

#include "../algorithms/bounds.h"
#include "../algorithms/cmathematics/vec.h"

// forward declaration
class CollisionModel;
class CollisionMesh;

typedef struct Face {
    CollisionMesh* mesh;
    unsigned int i1, i2, i3;

    vec baseNormal;
    vec norm;

    bool collidesWith(struct Face& face);
} Face;

class CollisionMesh {
public:
    CollisionModel* model;
    BoundingRegion br;
    RigidBody* rb;

    std::vector<vec> points;
    std::vector<Face> faces;

    CollisionMesh(unsigned int noPoints, float* coordinates, unsigned int noFaces, unsigned int* indices);

    bool collidesWith(CollisionMesh& mesh);

    void attachToRB(RigidBody* rb);

    void transform();
};

#endif