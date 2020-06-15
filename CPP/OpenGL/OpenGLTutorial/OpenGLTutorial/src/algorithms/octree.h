#ifndef OCTREE_H
#define OCTREE_H

#define NO_CHILDREN 8
#define MIN_BOUNDS 0.5

#include <vector>
#include <queue>
#include <stack>

#include "list.hpp"
#include "states.hpp"
#include "bounds.h"

namespace Octree {
	/*
		Active branch flags
	*/
	enum class Octant : unsigned char {
		O1 = 0x01,	// = 0b00000001
		O2 = 0x02,	// = 0b00000010
		O3 = 0x04,	// = 0b00000100
		O4 = 0x08,	// = 0b00001000
		O5 = 0x10,	// = 0b00010000
		O6 = 0x20,	// = 0b00100000
		O7 = 0x40,	// = 0b01000000
		O8 = 0x80	// = 0b10000000
	};

	/*
		Utility methods callbacks
	*/
	// calculate bounds of specified octant in bounded region and output
	void calculateBounds(BoundingRegion* out, Octant octant, BoundingRegion region);

	/*
		class to represent each node in the octree
	*/
	class node {
	public:
		node* parent; // pointer to parent
		node* children[NO_CHILDREN]; // array of children

		std::queue<BoundingRegion> queue; // queue of objects to be dynamically inserted
		std::vector<BoundingRegion> objects; // list of objects in node

		short maxLifespan = 8;
		short currentLifespan = -1;

		unsigned char activeOctants; // switch for active octants

		bool hasChildren = false; // if node has children

		bool treeReady = false; // if tree is ready
		bool treeBuilt = false; // if tree is built

		BoundingRegion region; // region of bounds of cell (AABB)

		// default constructor
		node();

		// initialize with bounds (no objects yet)
		node(BoundingRegion bounds);

		// initialize with bounds and list of objects
		node(BoundingRegion bounds, std::vector<BoundingRegion> objectList);

		// build tree (called during initialization)
		void build();

		// update objects in tree (called during each iteration of main loop)
		void update();

		// process pending queue
		void processPending();

		// dynamically insert object into node
		bool insert(BoundingRegion obj);

		// destroy object (free memory)
		void destroy();
	};
}

#endif