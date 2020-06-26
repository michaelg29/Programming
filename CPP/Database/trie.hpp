#ifndef TRIE_HPP
#define TRIE_HPP

#include <string>

namespace trie {
	// charsets for keys
	std::string ascii_letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	std::string ascii_lowercase = "abcdefghijklmnopqrstuvwxyz";
	std::string ascii_uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	std::string digits = "0123456789";
	std::string hex_digits = "0123456789abcdefABCDEF";
	std::string alpha_numeric = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

	template <typename T>
	struct node {
		bool exists;				// if data exists
		T data;						// data pointer
		struct node<T>** children;	// array of children

		// traverse this node and its children
		// send data to callback if exists
		void traverse(void(*itemViewer)(T data), unsigned int noChars) {
			if (exists) {
				itemViewer(data);
			}

			if (children) {
				for (int i = 0; i < noChars; i++) {
					if (children[i] != NULL) {
						children[i]->traverse(itemViewer, noChars);
					}
				}
			}
		}
	};

	template <typename T>
	class Trie {
	public:
		/*
			constructor
		*/
		Trie(std::string chars)
			: chars(chars), noChars(chars.length()), root(nullptr) {
			// initialize root memory
			root = (node<T>*)(malloc(sizeof(node<T>)));
			root->exists = false;
			root->children = new node<T> * [noChars];
			for (int i = 0; i < noChars; i++) {
				root->children[i] = NULL;
			}
		}

		/*
			modifiers
		*/
		// insertion (also can use to edit element)
		bool insert(std::string key, T element) {
			int idx;
			node<T>* current = root;

			for (char c : key) {
				idx = getIdx(c);
				if (idx == -1) {
					return false;
				}
				if (!current->children[idx] || current->children[idx] == NULL) {
					current->children[idx] = new node<T>;

					current->children[idx]->exists = false;
					current->children[idx]->children = new node<T> * [noChars];
					for (int i = 0; i < noChars; i++) {
						current->children[idx]->children[i] = NULL;
					}
				}
				current = current->children[idx];
			}

			current->data = element;
			current->exists = true;

			return true;
		}

		// deletion method
		bool erase(std::string key) {
			if (!root) {
				return false;
			}

			int idx;
			node<T>* current = root;

			int i, len;
			for (i = 0, len = key.length(); i < len - 1; i++) {
				idx = getIdx(key[i]);
				if (idx == -1) {
					return false;
				}
				if (!current->children[idx]) {
					return false;
				}
				current = current->children[idx];
			}

			idx = getIdx(key[i]);

			unloadNode(current->children[idx]);
			current->children[idx] = NULL;

			return true;
		}
		
		/*
			accesors
		*/
		// determine if key contained in trie
		bool containsKey(std::string key) {
			int idx;
			node<T>* current = root;
			for (char c : key) {
				idx = getIdx(c);

				if (idx == -1) {
					return false;
				}

				current = current->children[idx];
				if (current == NULL) {
					return false;
				}
			}
			return current->exists;
		}

		// obtain data element from node at key
		T& operator[](std::string key) {
			if (!containsKey(key)) {
				throw std::invalid_argument("key not found");
			}

			int idx;
			node<T>* current = root;
			for (char c : key) {
				idx = getIdx(c);

				if (idx == -1) {
					throw std::invalid_argument("invalid key");
				}

				current = current->children[idx];
				if (current == NULL) {
					throw std::invalid_argument("key not found");
				}
			}

			if (!current->exists) {
				throw std::invalid_argument("key not found");
			}

			return current->data;
		}

		// traverse through all keys
		void traverse(void(*itemViewer)(T data)) {
			if (root) {
				root->traverse(itemViewer, chars.length());
			}
		}

		// release root node
		void cleanup() {
			unloadNode(root);
		}

	private:
		// character set
		std::string chars;
		// length of character set
		unsigned int noChars;

		// root node
		node<T>* root;

		// get index of specific character in character set
		// return -1 if not found
		int getIdx(char c) {
			for (int i = 0, len = chars.length(); i < len; i++) {
				if (chars[i] == c) {
					return i;
				}
			}
			return -1;
		}

		// get character at specific index of character set
		// return space if invalid index
		char getChar(int i) {
			return (i >= 0 && i < chars.length()) ? chars[i] : ' ';
		}

		// unload node and its children
		void unloadNode(node<T>* top) {
			if (!top) {
				return;
			}

			for (int i = 0; i < noChars; i++) {
				if (top->children[i]) {
					unloadNode(top->children[i]);
				}
			}

			top = nullptr;
		}
	};
}

#endif