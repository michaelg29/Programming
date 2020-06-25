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

int main() {
	std::cout << "Hello, world!" << std::endl;

	//std::vector<data*> d = loadData("data.txt");

	/*std::vector<test*> elements;

	for (int i = 0; i < 10; i++) {
		test* t = new test;
		t->key = i;
		elements.push_back(t);
	}

	BTree<test> t(2);
	t.insert(elements);

	t.traverse();*/

	trie::Trie<test*> t(trie::ascii_lowercase, 26);

	t.insert("asdf", new test({ 5 }));

	bool b = t.containsKey("asdf");
	t.erase("asdf");
	b = t.containsKey("asdf");

	t.cleanup();

	std::cout << "goodbye" << std::endl;

	return 0;
}