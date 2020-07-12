#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "btree.hpp"
#include "trie.hpp"

struct data {
	unsigned int key;

	std::string name;
	std::string position;
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

int main() {
	std::cout << "Hello, world!" << std::endl;

	btree::BTree<data> tree(4);

	std::vector<unsigned int> keys = { 10, 80, 30, 90, 85, 40, 50, 60, 70, 95, 100 };
	//std::vector<unsigned int> keys = { 30, 50, 60, 65, 20, 35, 40, 55, 70, 10, 15, 25, 18, 21 };
	for (unsigned int i : keys) {
		data* d = new data;
		d->key = i;
		d->name = "name" + std::to_string(i);
		d->position = "pos" + std::to_string(i);
		tree.insert(d);
	}

	/*tree.erase(40);
	tree.erase(70);
	tree.erase(60);
	tree.erase(65);*/

	tree.traverse([](data* d) -> void {
		std::cout << d->key << ' ';
	});

	std::cout << std::endl;

	//tree.erase(50);

	tree.erase(30);
	tree.erase(10);
	tree.erase(60);
	tree.erase(50);

 	tree.traverse([](data* d) -> void {
		std::cout << d->key << ' ';
	});

	std::cout << std::endl;

	tree.clear();
	std::cout << "goodbye" << std::endl;

	return 0;
}