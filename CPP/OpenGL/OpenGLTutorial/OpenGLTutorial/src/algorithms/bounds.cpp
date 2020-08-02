#include "bounds.h"
#include "octree.h"

// override operator
bool BoundingRegion::operator==(BoundingRegion br) {
	if (type != br.type) {
		return false;
	}

	if (type == BoundTypes::AABB) {
		return (min == br.min) && (max == br.max);
	}
	else {
		return (center == br.center) && (radius == br.radius);
	}
}

// initialize with type
BoundingRegion::BoundingRegion(BoundTypes type)
	: type(type) {}

// initialize as sphere
BoundingRegion::BoundingRegion(glm::vec3 center, float radius)
	: center(center), ogCenter(center), radius(radius), ogRadius(radius), type(BoundTypes::SPHERE) {}

// initialize as AABB
BoundingRegion::BoundingRegion(glm::vec3 min, glm::vec3 max) 
	: min(min), ogMin(min), max(max), ogMax(max), type(BoundTypes::AABB) {}

// apply transformations of instance
void BoundingRegion::transform() {
	if (instance) {
		if (type == BoundTypes::AABB) {
			min = ogMin * instance->size + instance->pos;
			max = ogMax * instance->size + instance->pos;
		}
		else {
			center = ogCenter * instance->size + instance->pos;
			radius = ogRadius * instance->size.x;
		}
	}
}

// method to calculate center of region
glm::vec3 BoundingRegion::calculateCenter() {
	return (type == BoundTypes::AABB) ? (min + max) / 2.0f : center;
}

// method to calculate dimensions of region
glm::vec3 BoundingRegion::calculateDimensions() {
	return (type == BoundTypes::AABB) ? abs(max - min) : glm::vec3(2 * radius);
}

// method to determine if point is inside region
bool BoundingRegion::containsPoint(glm::vec3 pt) {
	if (type == BoundTypes::AABB) {
		// box - must be more than min and less than max
		return (pt.x >= min.x) && (pt.x <= max.x) &&
			(pt.y >= min.y) && (pt.y <= max.y) &&
			(pt.z >= min.z) && (pt.z <= max.z);
	}
	else {
		// sphere - distance must be less than radius
		// x^2 + y^2 + z^2 <= r^2
		glm::vec3 dist = pt - center;
		return (dist.x * dist.x +
			dist.y * dist.y +
			dist.z * dist.z)
			<= (radius * radius);
	}
}

// method to determine if bounding region is entirely inside
bool BoundingRegion::containsRegion(BoundingRegion br) {
	if (br.type == BoundTypes::AABB) {
		// if br is box, just has to contain both min and max
		return containsPoint(br.min) && containsPoint(br.max);
	}
	else if (type == BoundTypes::SPHERE && br.type == BoundTypes::SPHERE) {
		// if both spheres
		return glm::length(center - br.center) + br.radius < radius;
	}
	else {
		// if this a box and br is a sphere
		if (!containsPoint(br.center)) {
			// center outside of box
			return false;
		}

		// center inside box
		/*
			for each axis:
			- if distance to each side smaller than radius, return false
		*/

		for (int i = 0; i < 3; i++) {
			// don't have to worry about negative signs because we know the center is in the box
			float distMax = glm::length(max[i] - br.center[i]);
			float distMin = glm::length(br.center[i] - min[i]);
			if (distMax < br.radius || distMin < br.radius) {
				return false;
			}
		}

		return true;
	}
}

// method to determine if bounding region intersects
bool BoundingRegion::intersectsWith(BoundingRegion br) {
	// overlap on all axes

	if (type == BoundTypes::AABB && br.type == BoundTypes::AABB) {
		// both boxes

		glm::vec3 rad = calculateDimensions() / 2.0f;				// "radius" of this box
		glm::vec3 radBr = br.calculateDimensions() / 2.0f;			// "radius" of br

		glm::vec3 center = calculateCenter();						// center of this box
		glm::vec3 centerBr = br.calculateCenter();					// center of br

		glm::vec3 dist = abs(center - centerBr);

		for (int i = 0; i < 3; i++) {
			if (dist[i] > rad[i] + radBr[i]) {
				// no overlap on this axis
				return false;
			}
		}

		// failed to prove wrong on each axis
		return true;
	}
	else if (type == BoundTypes::SPHERE && br.type == BoundTypes::SPHERE) {
		// both spheres - distance between centers must be less than combined radius

		return glm::length(center - br.center) < (radius + br.radius);
	}
	else if (type == BoundTypes::SPHERE) {
		// this is a sphere, br is a box
		float distSquared = 0.0f;
		for (int i = 0; i < 3; i++) {
			// determine closest side
			float closestPt = std::max(br.min[i], std::min(center[i], br.max[i]));
			// add distance
			distSquared += (closestPt - center[i]) * (closestPt - center[i]);
		}

		return distSquared < (radius* radius);
	}
	else {
		// this is a box, br is a sphere
		// call algorithm for br (defined in preceding else if block)
		return br.intersectsWith(*this);
	}
}

// method to determine if ray intersects region
bool BoundingRegion::rayIntersects(glm::vec3 p1, glm::vec3 p2) {
	return true;
}