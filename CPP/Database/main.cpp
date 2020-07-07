#include <iostream>
#include <fstream>
#include <string>
#include "btree.hpp"
#include "trie.hpp"

struct data {
	unsigned int key;

	std::string name;
	std::string position;
};

struct test {
	unsigned int key;
};

std::vector<data*> loadData(const char* filename) {
	std::vector<data*> ret;

	std::ifstream file;
	file.open(filename);

	if (file.is_open()) {
		std::string name, position, temp;

		int i = 0;

		while (getline(file, name, ',')) {
			getline(file, position);

			data* d = new data;
			d->key = i;
			d->name = name;
			d->position = position;

			ret.push_back(d);

			i++;
		}
	}

	return ret;
}

bool writeData(const char* filename, std::vector<data*> elements) {
	std::ofstream file;
	file.open(filename);

	if (file.is_open()) {
		for (data* d : elements) {
			file << d->name << ',' << d->position << std::endl;
		}

		return true;
	}

	return false;
}

void printData(test* data) {
	std::cout << data->key << ' ';
}

int main() {
	std::cout << "Hello, world!" << std::endl;

	std::vector<data*> d = loadData("data.txt");

	std::vector<data*> elements;
	std::vector<unsigned int> keys = { 1, 50, 90, 40, 30, 35, 20, 10, 5, 2 };
	for (unsigned int i : keys) {
		data* t = new data;
		t->key = i;
		t->name = "name" + std::to_string(i);
		t->position = "pos" + std::to_string(i);
		elements.push_back(t);
	}

	btree::BTree<data> tree(4);
	tree.insert(elements);

	tree.erase(20);

	data* da = tree.root->children[0]->children[1]->data[1];
	std::cout << da->name << std::endl;

	std::vector<data*> res = tree.traverse();
	std::cout << std::endl;
	for (data* d : res) {
		std::cout << d->key << ' ';
	}
	std::cout << std::endl;

	std::cout << "goodbye" << std::endl;

	return 0;
}