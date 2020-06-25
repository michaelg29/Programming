#ifndef BTREE_HPP
#define BTREE_HPP

#include <vector>
#include <iostream>

template <typename T>
class node {
public:
	node** children;	// array of child pointers
	T** data;			// array of data (has keys)

	unsigned int t;		// minimum tree degree
	unsigned int n;		// current number of keys

	bool leaf;			// true if no children

	node* parent;		// pointer to parent

	node(unsigned int t, bool leaf = true)
		: t(t), leaf(leaf) {
		// allocate arrays
		children = new node*[2 * t];
		data = new T*[2 * t - 1];

		// set number of keys to 0
		n = 0;
	}

	void traverse() {
		int i;
		for (i = 0; i < n; i++) {
			if (!leaf) {
				children[i]->traverse();
			}
			std::cout << ' ' << data[i]->key;
		}

		if (!leaf) {
			children[i]->traverse();
		}
	}

	T* search(unsigned int idx) {
		return nullptr;
	}

	void splitChild(unsigned int i, node* left) {
		unsigned int divisor = t - 1;
		T* element = left->data[divisor];
		left->data[divisor] = nullptr;

		// generate node to be put on right
		node* right = new node(t, left->leaf);
		
		// move keys to right of divisor to right
		for (int j = divisor + 1; j < left->n; j++) {
			right->data[j - divisor - 1] = left->data[j];
			left->data[j] = nullptr;
		}
		right->n = left->n - divisor - 1;
		left->n = divisor;

		// move children to right of divisor to right
		if (!left->leaf) {
			// has children
			for (int j = divisor + 1; j < left->n + 1; j++) {
				right->children[j - divisor - 1] = left->children[j];
				left->children[j] = nullptr;
			}
		}

		// find slot for new child in this node
		int j;
		for (j = n; j >= i + 1; j--) {
			children[j + 1] = children[j];
		}
		children[j + 1] = right;

		// find location for key
		// start at right and loop until greater than a key
		for (j = n - 1; j >= 0 && data[j]->key > element->key; j--) {
			data[j + 1] = data[j];
		}

		data[j + 1] = element; // i + 1 because decremented in last for loop iteration
		n++;
	}

	bool insert(T* element) {
		if (search(element->key)) {
			// element already exists
			return false;
		}

		int i = n - 1;

		if (leaf) {
			/*
				is leaf
				- upshift all keys above insertion
				- insert element
			*/

			// start at right and loop until greater than a key
			for (; i >= 0 && data[i]->key > element->key; i--) {
				data[i + 1] = data[i];
			}

			data[i + 1] = element; // i + 1 because decremented in last for loop iteration
			n++;
		}
		else {
			/*
				not leaf
				- find corresponding child using ranges
			*/

			while (i >= 0 && data[i]->key > element->key) {
				i--;
			}

			// we know data[i] < element < data[i + 1]

			// pre-emptively split children
			if (children[i + 1]->n == t) {
				splitChild(i + 1, children[i + 1]);

				// find what child the new key goes to
				if (element->key > data[i + 1]->key) {
					// insert to right
					i++;
				}
			}

			// insert data
			children[i + 1]->insert(element);
		}

		return true;
	}

	bool rem(T* element) {
		if (!search(element->key)) {
			// element doesn't exist
			return false;
		}

		return true;
	}

	std::vector<T*> output() {
		return {};
	}

	void clear() {

	}
};

template <typename T>
class BTree {
public:
	unsigned int t;

	node<T>* root;

	BTree(unsigned int t)
		: t(t), root(nullptr) {}

	void traverse() {
		if (root) {
			root->traverse();
		}
	}

	void insert(std::vector<T*> elements) {
		unsigned int noElements = elements.size();
		if (noElements != 0) {
			for (T* element : elements) {
				insert(element);
				//traverse();
				//std::cout << std::endl;
			}
		}
	}

	void insert(T* element) {
		if (!root) {
			// root doesn't exist
			root = new node<T>(t);
			root->data[0] = element;
			root->n = 1;
		}
		else {
			if (root->n == t) {
				// root full, get more space

				node<T>* newRoot = new node<T>(t, false);

				// left child will be current
				newRoot->children[0] = root;

				// split current root into two
				newRoot->splitChild(0, root);
				newRoot->traverse();
				std::cout << std::endl;

				// place key accordingly
				int i = (element->key < newRoot->data[0]->key) ? 0 : 1;
				newRoot->children[i]->insert(element);

				// update root
				root = newRoot;
			}
			else {
				// root not full
				root->insert(element);
			}
		}
	}

	T* search(unsigned int key) {
		return (root) ? root->search(key) : nullptr;
	}

private:
	void updateRoot() {
		while (root->parent) {
			root = root->parent;
		}
	}
};

#endif