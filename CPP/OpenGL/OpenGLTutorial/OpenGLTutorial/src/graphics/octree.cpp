#include "octree.h"

bool Octree::Triangle::operator==(Triangle t2) {
	return p1 == t2.p1 &&
		p2 == t2.p2 &&
		p3 == t2.p3;
}

glm::vec3 Octree::BoundingBox::calculateCenter() {
	return (min + max) / 2.0f;
}

glm::vec3 Octree::BoundingBox::calculateDimensions() {
	return abs(max - min);
}

bool Octree::BoundingBox::containsTriangle(Triangle t) {
	return inBoundingBox(t.p1, min, max) &&
		inBoundingBox(t.p2, min, max) &&
		inBoundingBox(t.p3, min, max);
}

// check if point is in rectangle formed by min and max points
bool Octree::inBoundingBox(glm::vec3 pt, glm::vec3 min, glm::vec3 max) {
	return (pt.x > min.x) && (pt.x < min.x) &&
		(pt.y > min.y) && (pt.y < min.y) &&
		(pt.z > min.z) && (pt.z < min.z);
}

// find index of item in vector list
std::vector<Octree::Triangle>::iterator Octree::getIndexOf(std::vector<Triangle> v, Triangle x) {
	return std::find(v.begin(), v.end(), x);
}

// check if object in vector list
bool Octree::contains(std::vector<Triangle> v, Triangle x) {
	return getIndexOf(v, x) != v.end();
}

void Octree::calculateBounds(BoundingBox* out, unsigned char quadrant, BoundingBox region) {
	glm::vec3 center = region.calculateCenter();

	if (quadrant & ActiveBranches::Q1) {
		out = new BoundingBox({ center, region.max });
	}
	else if (quadrant & ActiveBranches::Q2) {
		out = new BoundingBox({ glm::vec3(region.min.x, center.y, center.z), glm::vec3(center.x, region.max.y, region.max.z) });
	}
	else if (quadrant & ActiveBranches::Q3) {
		out = new BoundingBox({ glm::vec3(region.min.x, region.min.y, center.z), glm::vec3(center.x, center.y, region.max.z) });
	}
	else if (quadrant & ActiveBranches::Q4) {
		out = new BoundingBox({ glm::vec3(center.x, region.min.y, center.z), glm::vec3(region.max.x, center.y, region.max.z) });
	}
	else if (quadrant & ActiveBranches::Q5) {
		out = new BoundingBox({ glm::vec3(center.x, center.y, region.min.z), glm::vec3(region.max.x, region.max.y, center.z) });
	}
	else if (quadrant & ActiveBranches::Q6) {
		out = new BoundingBox({ glm::vec3(region.min.x, center.y, region.min.z), glm::vec3(center.x, region.max.y, center.z) });
	}
	else if (quadrant & ActiveBranches::Q7) {
		out = new BoundingBox({ region.min,	center });
	}
	else if (quadrant & ActiveBranches::Q8) {
		out = new BoundingBox({ glm::vec3(center.x, region.min.y, region.min.z), glm::vec3(region.max.x, center.y, center.z) });
	}
}

Octree::node::node() {
	region = { glm::vec3(0.0f), glm::vec3(0.0f) };
}

Octree::node::node(BoundingBox bounds) {
	region = bounds;
}

Octree::node::node(BoundingBox bounds, std::vector<Triangle> objectList) {
	region = bounds;
	objects.insert(objects.end(), objectList.begin(), objectList.end());
}

void Octree::node::build() {
	/*
		termination conditions
		- no objects (ie an empty leaf node)
		- dimensions are less than MIN_BOUNDS
	*/
	if (objects.size() == 0) {
		return;
	}

	glm::vec3 dimensions = region.calculateDimensions();
	if (dimensions.x < MIN_BOUNDS && dimensions.y < MIN_BOUNDS && dimensions.z < MIN_BOUNDS) {
		return;
	}

	glm::vec3 center = region.calculateCenter();

	// create regions
	BoundingBox octants[NO_CHILDREN];
	for (int i = 0; i < NO_CHILDREN; i++) {
		calculateBounds(&octants[i], 1 << i, region);
	}

	std::vector<Triangle> octLists[NO_CHILDREN]; // array of lists of objects in each octant
	std::stack<int> delList; // list of objects that have been placed

	for (Triangle t : objects) {
		for (int i = 0; i < NO_CHILDREN; i++) {
			if (octants[i].containsTriangle(t)) {
				octLists[i].push_back(t);
				delList.push(i);
				break;
			}
		}
	}

	// remove objects from delList
	for (int i = 0, size = delList.size(); i < size; i++) {
		objects.erase(objects.begin() + delList.top());
		delList.pop();
	}

	for (int i = 0; i < NO_CHILDREN; i++) {
		if (octLists[i].size() != 0) {
			children[i] = new node(octants[i], octLists[i]);
			activeBranches |= (1 << i);
			/*
				1 = 00000001
				Shift 1 over i times (eg i = 3 --> 00001000)
				bitwise OR to combine
			*/
			children[i]->build();
			hasChildren = true;
		}
	}

	treeBuilt = true;
	treeReady = true;
}

void Octree::node::update() {
	if (treeBuilt && treeReady) {
		// start count down timer if no children or objects
		// when reaches zero, delete leaf
		// if counting down and new object enters, double lifespan
		if (objects.size() == 0) {
			if (!hasChildren) {
				// ensure doesn't have any leaves
				if (currentLifespan == -1) {
					currentLifespan = maxLifespan;
				}
				else if (currentLifespan > 0) {
					currentLifespan--;
				}
			}
		}
		else {
			if (currentLifespan != -1) {
				if (maxLifespan <= 64) {
					maxLifespan *= 2;
				}
				currentLifespan = -1;
			}
		}

		std::vector<Triangle> movedObjects(objects.size());
		// get moved objects that were in this leaf in the previous frame

		// remove objects that don't exist anymore
		for (int i = 0, listSize = objects.size(); i < listSize; i++) {
			// if doesn't exist, remove from objects and possibly moved objects
			/*
			if (!objects[i].alive) {
				if (contains(movedObjects, objects[i])) {
					movedObjects.erase(getIndexOf(movedObjects, objects[i]));
				}
				objects.erase(objects.begin() + i);
				i--;
				listSize--;
			}
			*/
		}

		// remove dead branches
		unsigned char flags = activeBranches;
		for (int i = 0;
			flags > 0;
			flags >>= 1, i++) {
			if ((flags & 1) == 1 && children[i]->currentLifespan == 0) {
				// active and run out of time
				if (children[i]->objects.size() > 0) {
					// branch is dead but has children, reset timer
					children[i]->currentLifespan = -1; // will reset to max lifespan
				}
				else {
					// branch is dead
					children[i] = nullptr;
					// turn off switch in active branches list
					activeBranches ^= 1 << i;
				}
			}
		}

		// update child nodes
		for (int flags = activeBranches, i = 0;
			flags > 0;
			flags >>= 1, i++) {
			if ((flags & 1) == 1) {
				// active
				if (children != nullptr && children[i] != nullptr) {
					// branch/leaf not null
					children[i]->update();
				}
			}
		}

		// move moved objects into new nodes
		Triangle movedObj;
		while (movedObjects.size() != 0) {
			/*
			for each moved object
			- traverse up tree (start with current node) until find a node that completely encloses moved object (can be current)
			- call insert (will push object as far down as possible, if went up will not push down)
			*/

			movedObj = movedObjects[0]; // get first item
			node* current = this;

			while (!current->region.containsTriangle(movedObj)) {
				if (current->parent != nullptr) {
					current = current->parent;
				}
				else {
					break; // if leave root node, break
				}
			}

			movedObjects.erase(movedObjects.begin());
			objects.erase(getIndexOf(objects, movedObj));
			current->insert(movedObj);
		}

		// collision detection

	}
	else {
		// process pending results
		if (pendingQueue.size() > 0) {
			processPending();
		}
	}
}

void Octree::node::processPending() {
	if (!treeBuilt) {
		// add objects to be sorted/organized into branches when built
		while (pendingQueue.size() != 0) {
			objects.push_back(pendingQueue.front());
			pendingQueue.pop();
		}
		build();
	}
	else {
		// insert pending objects immediately
		while (pendingQueue.size() != 0) {
			insert(pendingQueue.front());
			pendingQueue.pop();
		}
	}
}

bool Octree::node::insert(Triangle t) {
	/*
		termination conditions
		- no objects (ie an empty leaf node)
		- dimensions are less than MIN_BOUNDS
	*/
	// if empty, only object, so no need to divide, just push back
	if (objects.size() == 0) {
		objects.push_back(t);
		return true;
	}

	// if smallest size, just add because cannot divide
	glm::vec3 dimensions = region.calculateDimensions();
	if (dimensions.x <= MIN_BOUNDS && dimensions.y <= MIN_BOUNDS && dimensions.z <= MIN_BOUNDS) {
		objects.push_back(t);
		return true;
	}

	// if cannot fit into current, push up
	// should not happen, but a safeguard just incase
	if (!region.containsTriangle(t)) {
		return parent == nullptr ? false : parent->insert(t);
	}

	// now create region if not already defined
	BoundingBox octants[NO_CHILDREN];
	for (int i = 0; i < NO_CHILDREN; i++) {
		if (children[i] != nullptr) {
			calculateBounds(&octants[i], 1 << i, region);
		}
		else {
			octants[i] = { children[i]->region.min, children[i]->region.max };
		}
	}

	// find regions that fit the item entirely
	if (region.containsTriangle(t)) {
		bool found = false;
		for (int i = 0; i < 8; i++) {
			if (children[i]->region.containsTriangle(t)) {
				if (children[i] != nullptr) {
					return children[i]->insert(t);
				}
				else {
					children[i] = new node(octants[i], { t });
					activeBranches |= 1 << i;
					return true;
				}
			}
		}
	}
	return false;
}
