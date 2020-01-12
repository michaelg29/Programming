#include "shunting-yard.h"
#include "ref.h"
#include "utils/collections.h"

#include <stack>
#include <vector>
#include <string>

std::vector<std::string> RPN(const char* eqn) {
	std::vector<std::string> queue;
	std::stack<std::string> stack;

	std::string obj = "";
	char type = ' ';

	for (int i = 0, eq_len = strlen(eqn); i < eq_len; i++) {
		char t = eqn[i];

		// skip spaces/commas
		if (t == ' ' || t == ',') {
			continue;
		}

		// classify token
		if (isNumber(t)) {
			type = FuncTypes::CONST;

			int startI = i;
			if (i < eq_len - 1) {
				while (isNumber(eqn[i + 1])) {
					i++;
					if (i >= eq_len - 1) {
						break;
					}
				}
			}
			obj = std::string(eqn).substr(startI, i - startI + 1);
		} else {
			obj = findElement(i, eqn, functionNames);
			if (obj != "") {
				type = contains<char>(operators, obj[0]) ? FuncTypes::OP : FuncTypes::FUNCTION;
			} else {
				obj = findElement(i, eqn, constantNames);
				if (obj != "") {
					type = FuncTypes::CONST;
				} else {
					if (contains<char>(leftBrackets, t)) {
						type = FuncTypes::LPAREN;
						obj = "(";
					} else if (contains<char>(rightBrackets, t)) {
						type = FuncTypes::RPAREN;
						obj = ")";
					} else {
						type = FuncTypes::ELSE;
					}
				}
			}
			i += obj.size() - 1;
		}

		// do something with token
		const char* last_stack = (stack.size() > 0) ? stack.top().c_str() : "";
		switch (type) {
		case FuncTypes::CONST:
			queue.push_back(obj);
			break;
		case FuncTypes::FUNCTION:
			stack.push(obj);
			break;
		case FuncTypes::OP:
			if (stack.size() != 0) {
				while (
					((contains<std::string>(functionNames, last_stack) && !contains<char>(operators, last_stack[0])) ||
						getPrecedence(last_stack) > getPrecedence(obj) ||
						(getPrecedence(last_stack) == getPrecedence(obj)
							&& isLeftAssociative(last_stack))) &&
					!contains<char>(leftBrackets, last_stack[0])
					) {
					queue.push_back(stack.top());
					stack.pop();
					if (stack.size() == 0) {
						break;
					}
					last_stack = stack.top().c_str();
				}
			}
			stack.push(obj);
			break;
		case FuncTypes::LPAREN:
			stack.push("(");
			break;
		case FuncTypes::RPAREN:
			while (last_stack[0] != '(') {
				queue.push_back(stack.top());
				stack.pop();
				last_stack = stack.top().c_str();
			}
			stack.pop();
			break;
		default:
			return queue;
		}
	}

	while (stack.size() > 0) {
		queue.push_back(stack.top());
		stack.pop();
	}

	return queue;
}

Node* parse(std::vector<std::string> rpn) {
	std::stack<Node*> stack;

 	for (std::string item : rpn) {
		const char* t = item.c_str();
		if (isNumber(t)) {
			NumNode* ntr = new NumNode(t);
			stack.push(ntr);
		} else {
			FuncNode* ftr = new FuncNode(t);
			if (contains<std::string>(keys(binary_functions), t)) {
				ftr->setUnary(false);
				ftr->right = stack.top();
				stack.pop();
				ftr->left = stack.top();
				stack.pop();
			} else if (contains<std::string>(keys(unary_functions), t)) {
				ftr->setUnary(true);
				ftr->left = stack.top();
				stack.pop();
			}
			stack.push(ftr);
		}
	}

	if (stack.size() == 0) {
		return nullptr;
	}

	return stack.top();
}

double eval(Node* tree) {
	if (tree->isFunc) {
		FuncNode* ftree = (FuncNode*)tree;
		if (ftree->func.unary) {
			return ftree->eval(eval(tree->left));
		} else {
			return ftree->eval(eval(tree->left), eval(tree->right));
		}
	} else {
		NumNode* ntree = (NumNode*)tree;
		return ntree->eval();
	}
}
