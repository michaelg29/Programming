#include "node.h"

bool node::operator==(const node& n) const {
	return name == n.name;
}

bool node::operator!=(const node& n) const {
	return name != n.name;
}

double node::distance(node* n) {
	for (std::pair<node*, double> pair : neighbors) {
		if (pair.first == n) {
			return pair.second;
		}
	}
}

node* node::genNode(const char* name) {
	node* ret = new node;
	ret->name = name;

	return ret;
}

void node::free(node* n) {
	for (std::pair<node*, double> pair : n->neighbors) {
		if (pair.first) {
			node::free(pair.first);
		}
	}

	free(n);
}