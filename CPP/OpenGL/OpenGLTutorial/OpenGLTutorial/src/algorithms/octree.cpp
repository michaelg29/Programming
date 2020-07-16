#include "octree.h"
#include "../graphics/models/box.hpp"

// calculate bounds of specified quadrant in bounding region
void Octree::calculateBounds(BoundingRegion &out, Octant octant, BoundingRegion parentRegion)
{
	// find min and max points of corresponding octant

	glm::vec3 center = parentRegion.calculateCenter();
	if (octant == Octant::O1)
	{
		out = BoundingRegion(center, parentRegion.max);
	}
	else if (octant == Octant::O2)
	{
		out = BoundingRegion(glm::vec3(parentRegion.min.x, center.y, center.z), glm::vec3(center.x, parentRegion.max.y, parentRegion.max.z));
	}
	else if (octant == Octant::O3)
	{
		out = BoundingRegion(glm::vec3(parentRegion.min.x, parentRegion.min.y, center.z), glm::vec3(center.x, center.y, parentRegion.max.z));
	}
	else if (octant == Octant::O4)
	{
		out = BoundingRegion(glm::vec3(center.x, parentRegion.min.y, center.z), glm::vec3(parentRegion.max.x, center.y, parentRegion.max.z));
	}
	else if (octant == Octant::O5)
	{
		out = BoundingRegion(glm::vec3(center.x, center.y, parentRegion.min.z), glm::vec3(parentRegion.max.x, parentRegion.max.y, center.z));
	}
	else if (octant == Octant::O6)
	{
		out = BoundingRegion(glm::vec3(parentRegion.min.x, center.y, parentRegion.min.z), glm::vec3(center.x, parentRegion.max.y, center.z));
	}
	else if (octant == Octant::O7)
	{
		out = BoundingRegion(parentRegion.min, center);
	}
	else if (octant == Octant::O8)
	{
		out = BoundingRegion(glm::vec3(center.x, parentRegion.min.y, parentRegion.min.z), glm::vec3(parentRegion.max.x, center.y, center.z));
	}
}

// default constructor
Octree::node::node()
	: region(BoundTypes::AABB) {}

// initialize with bounds (no objects yet)
Octree::node::node(BoundingRegion bounds)
	: region(bounds) {}

// initialize with bounds and list of objects
Octree::node::node(BoundingRegion bounds, std::vector<BoundingRegion> objectList)
	: region(bounds)
{
	objects.insert(objects.end(), objectList.begin(), objectList.end());
}

// add to pending queue
void Octree::node::addToPending(RigidBody *instance, trie::Trie<Model *> models)
{
	// get all bounding regions
	for (BoundingRegion br : models[instance->modelId]->boundingRegions)
	{
		br.instance = instance;
		br.transform();
		queue.push(br);
	}
}

// build tree (called during initialization)
void Octree::node::build()
{
	/*
		termination conditions (don't subdivide further)
		- 1 or less objects (ie an empty leaf node or node with 1 object)
		- dimesnions are too small
	*/

	// initialize variables
	BoundingRegion octants[NO_CHILDREN];
	std::vector<BoundingRegion> octLists[NO_CHILDREN]; // array of lists of objects in each octant

	// too small
	glm::vec3 dimensions = region.calculateDimensions();
	for (int i = 0; i < 3; i++)
	{
		if (dimensions[i] < MIN_BOUNDS)
		{
			// set state variables
			goto setVars;
		}
	}

	// <= 1 objects
	if (objects.size() <= 1)
	{
		// set state variables
		goto setVars;
	}

	// create regions
	for (int i = 0; i < NO_CHILDREN; i++)
	{
		calculateBounds(octants[i], (Octant)(1 << i), region);
	}

	// determine which octants to place objects in
	for (int i = 0, length = objects.size(); i < length; i++)
	{
		BoundingRegion br = objects[i];
		for (int j = 0; j < NO_CHILDREN; j++)
		{
			if (octants[j].containsRegion(br))
			{
				// octant contains region
				octLists[j].push_back(br);
				objects.erase(objects.begin() + i);
				i--;
				length--;
				break;
			}
		}
	}

	// populate octants
	for (int i = 0; i < NO_CHILDREN; i++)
	{
		if (octLists[i].size() != 0)
		{
			// if children go into this octant
			children[i] = new node(octants[i], octLists[i]);
			children[i]->parent = this;
			States::activateIndex(&activeOctants, i); // activate octant
			children[i]->build();
			hasChildren = true;
		}
	}

	// set state variables
setVars:
	treeBuilt = true;
	treeReady = true;

	for (int i = 0; i < objects.size(); i++) {
		objects[i].cell = this;
	}
}

// update objects in tree (called during each iteration of main loop)
void Octree::node::update(Box &box)
{
	if (treeBuilt && treeReady)
	{
		box.positions.push_back(region.calculateCenter());
		box.sizes.push_back(region.calculateDimensions());

		// start count down timer if no children or objects
		// when reaches zero, delete leaf
		// if counting down and new object enters, double lifespan
		if (objects.size() == 0)
		{
			if (!hasChildren)
			{
				// ensure doesn't have any leaves
				if (currentLifespan == -1)
				{
					currentLifespan = maxLifespan;
				}
				else if (currentLifespan > 0)
				{
					currentLifespan--;
				}
			}
		}
		else
		{
			if (currentLifespan != -1)
			{
				if (maxLifespan <= 64)
				{
					maxLifespan *= 2;
				}
				currentLifespan = -1;
			}
		}

		// remove objects that don't exist anymore
		for (int i = 0, listSize = objects.size(); i < listSize; i++)
		{
			// if doesn't exist, remove from objects
			if (States::isActive(&objects[i].instance->state, INSTANCE_DEAD))
			{
				objects.erase(objects.begin() + i);
				// update counter/size var accordingly
				i--;
				listSize--;
			}
		}

		// get moved objects that were in this leaf in previous frame
		std::stack<int> movedObjects;
		for (int i = 0, listSize = objects.size(); i < listSize; i++)
		{
			if (States::isActive(&objects[i].instance->state, INSTANCE_MOVED))
			{
				objects[i].transform();
				movedObjects.push(i);
			}
			box.positions.push_back(objects[i].calculateCenter());
			box.sizes.push_back(objects[i].calculateDimensions());
		}

		// remove dead branches
		unsigned char flags = activeOctants;
		for (int i = 0;
			 flags > 0;
			 flags >>= 1, i++)
		{
			if (States::isIndexActive(&flags, 0) && children[i]->currentLifespan == 0)
			{
				// active and run out of time
				if (children[i]->objects.size() > 0)
				{
					// branch is dead but has children, reset timer
					children[i]->currentLifespan = -1; // will reset to max lifespan
				}
				else
				{
					// branch is dead
					children[i] = nullptr;
					// turn off switch in active branches list
					States::deactivateIndex(&activeOctants, i);
					//Quadrants ^= 1 << i;
					if (activeOctants == 0) {
						hasChildren = false;
					}
				}
			}
		}

		// update child nodes
		if (children)
		{
			// go through each octant using flags
			for (unsigned char flags = activeOctants, i = 0;
				 flags > 0;
				 flags >>= 1, i++)
			{
				if (States::isIndexActive(&flags, 0))
				{
					// active octant
					if (children[i] != nullptr)
					{
						// child not null
						children[i]->update(box);
					}
				}
			}
		}

		// move moved objects into new nodes
		BoundingRegion movedObj; // placeholder
		while (movedObjects.size() != 0)
		{
			/*
				for each moved object
				- traverse up tree (start with current node) until find a node that completely encloses the object
				- call insert (push object as far down as possible)
			*/

			movedObj = objects[movedObjects.top()]; // set to first object in list
			node *current = this;					// placeholder

			while (!current->region.containsRegion(movedObj))
			{
				if (current->parent)
				{
					// set current to current's parent (recursion)
					current = current->parent;
				}
				else
				{
					break; // if root node, then leave
				}
			}

			/*
				once finished
				- remove from objects list
				- remove from movedObjects stack
				- insert into found region
			*/
			objects.erase(objects.begin() + movedObjects.top());
			movedObjects.pop();
			movedObj.cell = current;
			current->insert(movedObj);

			// collision detection
			/*
				check objects in its parents, cell and children
			*/

			// itself
			current = movedObj.cell;
			current->checkCollisionSelf(movedObj);

			// parents
			while (current->parent) {
				current = current->parent;
				current->checkCollisionSelf(movedObj);
			}

			// children
			current = movedObj.cell;
   			current->checkCollisionChildren(movedObj);
		}
	}
	else
	{
		// process pending results
		if (queue.size() > 0)
		{
			processPending();
		}
	}
}

// process pending queue
void Octree::node::processPending()
{
	if (!treeBuilt)
	{
		// add objects to be sorted into branches when built
		while (queue.size() != 0)
		{
			objects.push_back(queue.front());
			queue.pop();
		}
		build();
	}
	else
	{
		for (int i = 0, len = queue.size(); i < len; i++) {
			BoundingRegion br = queue.front();
			if (region.containsRegion(br)) {
				// insert object immediately
				insert(br);
			}
			else {
				// else return to queue after updating position
				br.transform();
				queue.push(br);
			}
			queue.pop();
		}
	}
}

// dynamically insert object into node
bool Octree::node::insert(BoundingRegion obj)
{
	/*
		termination conditions
		- no objects (an empty leaf node)
		- dimensions are less than MIN_BOUNDS
	*/

	glm::vec3 dimensions = region.calculateDimensions();
	if (objects.size() == 0 ||
		dimensions.x < MIN_BOUNDS ||
		dimensions.y < MIN_BOUNDS ||
		dimensions.z < MIN_BOUNDS)
	{
		objects.push_back(obj);
		objects[0].cell = this;
		return true;
	}

	// safeguard if object doesn't fit
	if (!region.containsRegion(obj))
	{
		return parent == nullptr ? false : parent->insert(obj);
	}

	// create regions if not defined
	BoundingRegion octants[NO_CHILDREN];
	for (int i = 0; i < NO_CHILDREN; i++)
	{
		if (children[i] != nullptr)
		{
			// child exists, so take its region
			octants[i] = children[i]->region;
		}
		else
		{
			// get region for this octant
			calculateBounds(octants[i], (Octant)(1 << i), region);
		}
	}

	objects.push_back(obj);

	// determine which octants to place objects in
	std::vector<BoundingRegion> octLists[NO_CHILDREN]; // array of lists of objects in each octant
	//std::stack<int> delList;						   // list of objects that have been placed

	for (int i = 0, length = objects.size(); i < length; i++)
	{
		for (int j = 0; j < NO_CHILDREN; j++)
		{
			if (octants[j].containsRegion(objects[i]))
			{
				// octant contains region
				octLists[j].push_back(objects[i]);
				objects.erase(objects.begin() + i);
				i--;
				length--;
				break;
			}
		}
	}

	// find region that fits item entirely
	for (int i = 0; i < NO_CHILDREN; i++)
	{
		if (octLists[i].size() != 0)
		{
			if (children[i])
			{
				// insert into existing child
				for (BoundingRegion br : octLists[i]) {
					br.cell = children[i];
					children[i]->insert(br);
				}
			}
			else
			{
				// create node for child
				children[i] = new node(octants[i], octLists[i]);
				children[i]->parent = this;
				States::activateIndex(&activeOctants, i);
				children[i]->build();
				hasChildren = true;
			}
		}
	}

	return true;
}

// check collisions with all objects in node
void Octree::node::checkCollisionSelf(BoundingRegion obj) {
	if (objects.size() == 0) {
		return;
	}
	for (BoundingRegion br : objects) {
		if (br.intersectsWith(obj)) {
			if (br.instance->instanceId != obj.instance->instanceId) {
				std::cout << "Instance " << br.instance->instanceId << " (" << br.instance->modelId << ") intersects with " << obj.instance->instanceId << "(" << obj.instance->modelId << ")" << std::endl;
			}
		}
	}
}

// check collisions with all objects in child nodes
void Octree::node::checkCollisionChildren(BoundingRegion obj) {
	if (children)
	{
		for (int flags = activeOctants, i = 0;
			flags > 0;
			flags >>= 1, i++)
		{
			if (States::isIndexActive(&flags, 0)) {
				if (children[i]) {
					children[i]->checkCollisionSelf(obj);
					children[i]->checkCollisionChildren(obj);
				}
			}
		}
	}
}

// destroy object (free memory)
void Octree::node::destroy()
{
	// clearing out children
	if (children)
	{
		for (int flags = activeOctants, i = 0;
			 flags > 0;
			 flags >>= 1, i++)
		{
			if (States::isIndexActive(&flags, 0))
			{
				// active
				if (children[i] != nullptr)
				{
					children[i]->destroy();
					children[i] = nullptr;
				}
			}
		}
	}

	// clear this node
	objects.clear();
	while (queue.size() != 0)
	{
		queue.pop();
	}
}