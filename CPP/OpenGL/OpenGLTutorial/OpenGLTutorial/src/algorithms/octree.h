#ifndef OCTREE_H
#define OCTREE_H

#define NO_CHILDREN 8
#define MIN_BOUNDS 0.5

#include <vector>
#include <queue>
#include <stack>
#include <glm/glm.hpp>

#include "list.hpp"
#include "states.hpp"
#include "bounds.h"

namespace Octree {
	/*
		Active branch flags
	*/
	enum class Quadrants : unsigned char {
		Q1 = 0x01,	// = 0b00000001
		Q2 = 0x02,	// = 0b00000010
		Q3 = 0x04,	// = 0b00000100
		Q4 = 0x08,	// = 0b00001000
		Q5 = 0x10,	// = 0b00010000
		Q6 = 0x20,	// = 0b00100000
		Q7 = 0x40,	// = 0b01000000
		Q8 = 0x80	// = 0b10000000
	};

	/*
		Utility methods callbacks
	*/
	// calculate bounds of specified quadrant in bounded region and output
	void calculateBounds(BoundingRegion* out, unsigned char quadrant, BoundingRegion region);

	class node {
	public:
		node* parent;

		std::vector<BoundingRegion> objects;

		node* children[NO_CHILDREN];

		short maxLifespan = 8;
		short currentLifespan = -1;

		unsigned char Quadrants;

		bool hasChildren = false;

		bool treeReady = false;
		bool treeBuilt = false;

		std::queue<BoundingRegion> pendingQueue;

		BoundingRegion region;

		node();

		node(BoundingRegion bounds);

		node(BoundingRegion bounds, std::vector<BoundingRegion> objectList);

		void build();

		void update();

		void processPending();

		bool insert(BoundingRegion t);

		void destroy();
	};
}

#endif