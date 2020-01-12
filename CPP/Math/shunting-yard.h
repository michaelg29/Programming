#include <vector>
#include <string>

#include "utils/node.h"

#ifndef SHUNTING_YARD
#define SHUNTING_YARD

std::vector<std::string> RPN(const char* eqn);
Node* parse(std::vector<std::string> rpn);
double eval(Node* tree);

#endif