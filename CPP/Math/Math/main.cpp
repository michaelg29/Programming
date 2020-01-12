#include <iostream>
#include <vector>
#include <string>

#include "ref.h"
#include "utils/node.h"
#include "shunting-yard.h"

int main() {
	initRef();

    std::cout << "Input equation: ";
    
	std::string eqn;
	std::getline(std::cin, eqn);

	std::vector<std::string> rpn = RPN(eqn.c_str());
	Node* tree = parse(rpn);
	std::cout << '=' << eval(tree) << std::endl;

	return 0;
}
