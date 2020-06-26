#ifndef BTREE_HPP
#define BTREE_HPP

#include <vector>
#include <iostream>

template <typename T>
class node {
public:
	node<T>** children;		// array of child pointers
	T** data;				// array of data (has keys)

	int m;			// maximum tree degree
	int divisor;	// index divisor
	int n;			// current number of keys

	bool leaf;				// true if no children

	node(int m, bool leaf = true)
		: m(m), divisor(m / 2), leaf(leaf), n(0) {
		// allocate memory for arrays
		children = new node<T> * [m + 1];
		data = new T * [m];
	}

	node<T>* split(T*& upshiftElement) {
		node<T>* right = new node<T>(m, leaf);

		upshiftElement = data[divisor];
		data[divisor] = nullptr;

		// data
		for (int idx = divisor + 1; idx < n; idx++) {
			T* b = data[idx];
			right->data[idx - divisor - 1] = data[idx];
			right->n++;
			data[idx] = nullptr;
		}
		n -= right->n + 1; // to account for divisor

		// children
		if (!leaf) {
			// m + 1 because overflow creates one additional child
			for (int idx = divisor + 1; idx < m + 1; idx++) {
				node<T>* b = children[idx];
				right->children[idx - divisor - 1] = children[idx];
				children[idx] = nullptr;
			}
		}

		return right;
	}

	template <typename V>
	void shiftRight(V* arr, int i, int& size) {
		for (int idx = size - 1; idx >= i; idx--) {
			V b = arr[idx];
			arr[idx + 1] = arr[idx];
		}
		size++;
	}

	node<T>* insert(T* element, T*& upshiftElement) {
		/*
			if full, return branch to right
		*/

		/*
			find ordered position for element
			- start from left and iterate until is less than an element
			- its location will be i for data after shift, i for children
		*/
		int i = 0;
		while (i < n && element->key > data[i]->key) {
			i++;
		}

		if (leaf) {
			shiftRight<T*>(data, i, n);
			data[i] = element;

			if (n == m) {
				// too many elements
				return split(upshiftElement);
			}
		}
		else {
			// pass down to child
			node<T>* right = children[i]->insert(element, upshiftElement);

			if (right) {
				// child overflowed, returned divisor and right branch

				// set data
				shiftRight<T*>(data, i, n);
				data[i] = upshiftElement;

				// add child
				int j = n;
				shiftRight<node<T>*>(children, i + 1, j);
				children[i + 1] = right;

				if (n == m) {
					// too many elements
					return split(upshiftElement);
				}
			}
		}

		upshiftElement = nullptr;
		return nullptr;
	}

	bool erase(int key) {
		/*
			cases

		*/

		// find location
		bool found = false;
		int i = 0;
		while (i < n && key >= data[i]->key) {
			if (key == data[i]->key) {
				// found element
				found = true;
				break;
			}
			i++;
		}

		if (found) {
			if (leaf) {
				// simply delete
				data[i] = nullptr;
			}
			else {

			}
		}
		else {
			// if no children, no other places to look for the element
			return leaf ? false : children[i]->erase(key);
		}
	}

	T* search(int key) {
		int i = 0;
		while (i < n && key >= data[i]->key) {
			if (key == data[i]->key) {
				// found element
				return data[i];
			}
			i++;
		}

		// know not in data

		// if no children, no other places to look for the element
		return leaf ? nullptr : children[i]->search(key);
	}

	std::vector<T*> traverse() {
		std::vector<T*> ret;
		for (int i = 0; i < n; i++) {
			if (!leaf) {
				std::vector<T*> res = children[i]->traverse();
				ret.insert(ret.end(), res.begin(), res.end());
			}
			ret.push_back(data[i]);
		}
		if (!leaf) {
			std::vector<T*> res = children[n]->traverse();
			ret.insert(ret.end(), res.begin(), res.end());
		}

		return ret;
	}

	void clear();
};

template <typename T>
class BTree {
public:
	int m;

	node<T>* root;

	BTree(int m)
		: m(m) {
		root = new node<T>(m, true);
	}

	std::vector<T*> traverse() {
		return root->traverse();
	}

	void insert(std::vector<T*> elements) {
		for (T* element : elements) {
			insert(element);
		}
	}

	void insert(T* element) {
		T* upshiftElement = nullptr;
		node<T>* right = root->insert(element, upshiftElement);

		if (right) {
			// need to create new root
			node<T>* newRoot = new node<T>(m, false);
			
			// set data
			newRoot->data[0] = upshiftElement;
			newRoot->n = 1;

			// place children
			newRoot->children[0] = root;
			newRoot->children[1] = right;

			// set root
			root = newRoot;
		}
	}

	T* search(int key) {
		return root->search(key);
	}
};

#endif