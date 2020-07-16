#ifndef BOUNDS_H
#define BOUNDS_H

#include <glm/glm.hpp>
#include <string>

#include "../physics/rigidbody.h"

namespace Octree {
	class node;
}

enum class BoundTypes : unsigned char {
	AABB = 0x01,
	SPHERE = 0x02
};

class BoundingRegion {
public:
	BoundTypes type;

	// rigid body pointer (for quick access to state)
	RigidBody* instance;

	// octree cell pointer (for quick access to current cell)
	Octree::node* cell;

	// sphere values
	glm::vec3 center;
	float radius;

	glm::vec3 ogCenter;
	float ogRadius;

	// AABB values
	glm::vec3 min;
	glm::vec3 max;

	glm::vec3 ogMin;
	glm::vec3 ogMax;

	// override operator
	bool operator==(BoundingRegion br);

	// initialize with type
	BoundingRegion(BoundTypes type = BoundTypes::AABB);

	// initialize as sphere
	BoundingRegion(glm::vec3 center, float radius);

	// initialize as AABB
	BoundingRegion(glm::vec3 min, glm::vec3 max);

	// apply transformations of instance
	void transform();

	// method to calculate center of region
	glm::vec3 calculateCenter();

	// method to calculate dimensions of region
	glm::vec3 calculateDimensions();

	// method to determine if point is inside region
	bool containsPoint(glm::vec3 pt);

	// method to determine if bounding region is entirely inside
	bool containsRegion(BoundingRegion br);

	// method to determine if bounding region intersects
	bool intersectsWith(BoundingRegion br);

	// method to determine if ray intersects region
	bool rayIntersects(glm::vec3 p1, glm::vec3 p2);
};

#endif