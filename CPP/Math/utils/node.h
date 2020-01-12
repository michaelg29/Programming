#include "../func.h"

#ifndef NODE
#define NODE

//template<class T>
//struct node {
//	T val;
//	bool isFunc;
//	bool isUnary;
//	
//	struct node* right;
//	struct node* left;
//
//	const char* name;
//};
//
//struct node<Func>* genFuncNode(const char* name, Func func);
//struct node<double>* genNumNode(const char* name, double val);

class Node {
public:
	Node(const char* name, bool isFunc);

	double eval(double x = 0, double y = 0);

	const char* name;
	bool isFunc;

	Node* right;
	Node* left;
};

class FuncNode : public Node {
public:
	FuncNode(const char* name);

	void setUnary(bool isUnary);
	double eval(double x, double y = 0);

	Func func;
	bool isUnary;
};

class NumNode : public Node {
public:
	NumNode(const char* name);

	double eval(double x = 0, double y = 0);

	double val;
};

#endif