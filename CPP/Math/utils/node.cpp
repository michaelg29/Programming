#include "node.h"

#include "../func.h"
#include "../ref.h"

Node::Node(const char* name, bool isFunc)
	: name(name), isFunc(isFunc) {}

FuncNode::FuncNode(const char* name)
	: Node(name, true) {}

void FuncNode::setUnary(bool isUnary) {
	this->isUnary = isUnary;

	if (isUnary) {
		this->func = unary_functions[name];
	}
	else {
		this->func = binary_functions[name];
	}
}

double FuncNode::eval(double x, double y) {
	return this->func.eval(x, y);
}

NumNode::NumNode(const char* name)
	: Node(name, false) {
	this->val = getNumericalVal(name);
}

double NumNode::eval(double x, double y) {
	return this->val;
}