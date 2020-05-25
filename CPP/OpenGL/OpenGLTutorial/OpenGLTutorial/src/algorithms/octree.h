#ifndef OCTREE_H
#define OCTREE_H

#define NO_CHILDREN 8
#define MIN_BOUNDS 0.5

#include <vector>
#include <queue>
#include <stack>
#include <glm/glm.hpp>

namespace Octree {
	/*
		Triangle structure
	*/
	struct Triangle {
		unsigned int modelId;
		glm::vec3 p1, p2, p3;

		// have offset so can identify locally within model
		glm::vec3 offset;

		bool operator==(Triangle t2);
	};
	typedef struct Triangle Triangle;

	/*
		Bounding box structure (defined by min and max points)
	*/
	struct BoundingBox {
		glm::vec3 min;
		glm::vec3 max;

		glm::vec3 calculateCenter();

		glm::vec3 calculateDimensions();

		bool containsTriangle(Triangle t);
	};
	typedef struct BoundingBox BoundingBox;

	/*
		Active branch flags
	*/
	enum ActiveBranches {
		Q1 = 0x01,
		Q2 = 0x02,
		Q3 = 0x04,
		Q4 = 0x08,
		Q5 = 0x10,
		Q6 = 0x20,
		Q7 = 0x40,
		Q8 = 0x80
	};

	/*
		Utility methods callbacks
	*/
	// check if point is in rectangle formed by min and max points
	bool inBoundingBox(glm::vec3 pt, glm::vec3 min, glm::vec3 max);

	// find index of item in vector list
	std::vector<Triangle>::iterator getIndexOf(std::vector<Triangle> v, Triangle x);
	// find index of mdoel id in vector list
	std::vector<unsigned int>::iterator getIndexOf(std::vector<unsigned int> v, unsigned int x);

	// check if object in vector list
	bool contains(std::vector<Triangle> v, Triangle x);
	// check if model id in list
	bool contains(std::vector<unsigned int> v, int x);

	// calculate bounds of specified quadrant in bounded region and output
	void calculateBounds(BoundingBox* out, unsigned char quadrant, BoundingBox region);

	class node {
	public:
		node* parent;

		std::vector<Triangle> objects;

		node* children[NO_CHILDREN];

		short maxLifespan = 8;
		short currentLifespan = -1;

		unsigned char activeBranches;

		bool hasChildren = false;

		bool treeReady = false;
		bool treeBuilt = false;

		std::queue<Triangle> pendingQueue;

		BoundingBox region;

		node();

		node(BoundingBox bounds);

		node(BoundingBox bounds, std::vector<Triangle> objectList);

		void build();

		void update();

		void processPending();

		bool insert(Triangle t);
	};
}

#endif