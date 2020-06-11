#include "octree.h"

void Octree::calculateBounds(BoundingRegion* out, unsigned char quadrant, BoundingRegion region) {
	glm::vec3 center = region.calculateCenter();
	if (quadrant == (unsigned char)Quadrants::Q1) {
		out = new BoundingRegion(center, region.max);
	}
	else if (quadrant == (unsigned char)Quadrants::Q2) {
		out = new BoundingRegion(glm::vec3(region.min.x, center.y, center.z), glm::vec3(center.x, region.max.y, region.max.z));
	}
	else if (quadrant == (unsigned char)Quadrants::Q3) {
		out = new BoundingRegion(glm::vec3(region.min.x, region.min.y, center.z), glm::vec3(center.x, center.y, region.max.z));
	}
	else if (quadrant == (unsigned char)Quadrants::Q4) {
		out = new BoundingRegion(glm::vec3(center.x, region.min.y, center.z), glm::vec3(region.max.x, center.y, region.max.z));
	}
	else if (quadrant == (unsigned char)Quadrants::Q5) {
		out = new BoundingRegion(glm::vec3(center.x, center.y, region.min.z), glm::vec3(region.max.x, region.max.y, center.z));
	}
	else if (quadrant == (unsigned char)Quadrants::Q6) {
		out = new BoundingRegion(glm::vec3(region.min.x, center.y, region.min.z), glm::vec3(center.x, region.max.y, center.z));
	}
	else if (quadrant == (unsigned char)Quadrants::Q7) {
		out = new BoundingRegion(region.min, center);
	}
	else if (quadrant == (unsigned char)Quadrants::Q8) {
		out = new BoundingRegion(glm::vec3(center.x, region.min.y, region.min.z), glm::vec3(region.max.x, center.y, center.z));
	}
}

Octree::node::node() 
	: region(BoundTypes::AABB) {}

Octree::node::node(BoundingRegion bounds)
	: region(bounds) {}

Octree::node::node(BoundingRegion bounds, std::vector<BoundingRegion> objectList)
	: region(bounds) {
	objects.insert(objects.end(), objectList.begin(), objectList.end());
}

void Octree::node::build() {
	/*
		termination conditions
		- no objects (ie an empty leaf node)
		- dimensions are less than MIN_BOUNDS
	*/

	// no objects
	if (objects.size() == 0) {
		return;
	}

	// too small
	glm::vec3 dimensions = region.calculateDimensions();
	if (dimensions.x < MIN_BOUNDS && dimensions.y < MIN_BOUNDS && dimensions.z < MIN_BOUNDS) {
		return;
	}

	glm::vec3 center = region.calculateCenter();

	// create regions
	BoundingRegion octants[NO_CHILDREN];
	for (int i = 0; i < NO_CHILDREN; i++) {
		calculateBounds(&octants[i], (1 << i), region);
	}

	std::vector<BoundingRegion> octLists[NO_CHILDREN]; // array of lists of objects in each octant
	std::stack<int> delList; // list of objects that have been placed

	for (BoundingRegion br : objects) {
		for (int i = 0; i < NO_CHILDREN; i++) {
			if (octants[i].contains(br)) {
				octLists[i].push_back(br);
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
			States::activate(&Quadrants, i);
			//Quadrants |= 1 << i;
			children[i]->build();
			hasChildren = true;
		}
	}

	treeBuilt = true;
	treeReady = true;
}

void Octree::node::update() {
	if (treeBuilt && treeReady) {
		/*
		
			TODO: IGNORE
		
		*/

		// start count down timer if no children or objects
		// when reaches zero, delete leaf
		// if counting down and new object enters, double lifespan
		//if (objects.size() == 0) {
		//	if (!hasChildren) {
		//		// ensure doesn't have any leaves
		//		if (currentLifespan == -1) {
		//			currentLifespan = maxLifespan;
		//		}
		//		else if (currentLifespan > 0) {
		//			currentLifespan--;
		//		}
		//	}
		//}
		//else {
		//	if (currentLifespan != -1) {
		//		if (maxLifespan <= 64) {
		//			maxLifespan *= 2;
		//		}
		//		currentLifespan = -1;
		//	}
		//}

		// get moved objects that were in this leaf in the previous frame
		std::vector<BoundingRegion> movedObjects(objects.size());
		//std::vector<unsigned int> movedModels; // get from somewhere
		//for (BoundingRegion t : objects) {
		//	if (List::contains<unsigned int>(movedModels, t.modelId)) {
		//		movedObjects.push_back(t);
		//	}
		//}

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
		for (int i = 0, unsigned char flags = Quadrants;
			flags > 0;
			flags >>= 1, i++) {
			if (States::isActive(&flags, 0) && children[i]->currentLifespan == 0) {
				// active and run out of time
				if (children[i]->objects.size() > 0) {
					// branch is dead but has children, reset timer
					children[i]->currentLifespan = -1; // will reset to max lifespan
				}
				else {
					// branch is dead
					children[i] = nullptr;
					// turn off switch in active branches list
					States::deactivate(&Quadrants, i);
					//Quadrants ^= 1 << i;
				}
			}
		}

		// update child nodes
		for (int flags = Quadrants, i = 0;
			flags > 0;
			flags >>= 1, i++) {
			if (States::isActive(&flags, 0)) {
				// active
				if (children != nullptr && children[i] != nullptr) {
					// branch/leaf not null
					children[i]->update();
				}
			}
		}

		// move moved objects into new nodes
		BoundingRegion movedObj;
		while (movedObjects.size() != 0) {
			/*
			for each moved object
			- traverse up tree (start with current node) until find a node that completely encloses moved object (can be current)
			- call insert (will push object as far down as possible, if went up will not push down)
			*/

			movedObj = movedObjects[0]; // get first item
			node* current = this;

			while (!current->region.contains(movedObj)) {
				if (current->parent != nullptr) {
					current = current->parent;
				}
				else {
					break; // if leave root node, break
				}
			}

			// remove first object, second now becomes first
			movedObjects.erase(movedObjects.begin());
			objects.erase(objects.begin() + List::getIndexOf<BoundingRegion>(objects, movedObj));
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

bool Octree::node::insert(BoundingRegion br) {
	/*
		termination conditions
		- no objects (ie an empty leaf node)
		- dimensions are less than MIN_BOUNDS
	*/
	// if empty, only object, so no need to divide, just push back
	if (objects.size() == 0) {
		objects.push_back(br);
		return true;
	}

	// if smallest size, just add because cannot divide further
	glm::vec3 dimensions = region.calculateDimensions();
	if (dimensions.x <= MIN_BOUNDS && dimensions.y <= MIN_BOUNDS && dimensions.z <= MIN_BOUNDS) {
		objects.push_back(br);
		return true;
	}

	// if cannot fit into current, push up
	// should not happen, but a safeguard just incase
	if (!region.contains(br)) {
		return parent == nullptr ? false : parent->insert(br);
	}

	// now create region if not already defined
	BoundingRegion octants[NO_CHILDREN];
	for (int i = 0; i < NO_CHILDREN; i++) {
		if (children[i] != nullptr) {
			calculateBounds(&octants[i], (1 << i), region);
		}
		else {
			octants[i] = BoundingRegion(children[i]->region.min, children[i]->region.max);
		}
	}

	// find regions that fit the item entirely
	if (region.contains(br)) {
		bool found = false;
		for (int i = 0; i < 8; i++) {
			if (children[i]->region.contains(br)) {
				if (children[i] != nullptr) {
					return children[i]->insert(br);
				}
				else {
					children[i] = new node(octants[i], { br });
					States::activate(&Quadrants, i);
					return true;
				}
			}
		}
	}
	return false;
}

void Octree::node::destroy() {
	if (children != nullptr) {
		for (int flags = Quadrants, i = 0;
			flags > 0;
			flags >>= 1, i++) {
			if (States::isActive(&flags, 0)) {
				// active
				if (children[i] != nullptr) {
					// branch/leaf not null
					children[i]->destroy();
					children[i] = nullptr;
				}
			}
		}
	}

	objects.clear();
	while (pendingQueue.size() > 0) {
		pendingQueue.pop();
	}
}