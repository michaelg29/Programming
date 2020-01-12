#include "ref.h"
#include "utils/nums.h"
#include "utils/collections.h"

#include <vector>
#include <map>
#include <cmath>

std::map<std::string, Func> unary_functions;
std::map<std::string, Func> binary_functions;

std::vector<std::string> functionNames;

std::map<std::string, double> constants;
std::vector<std::string> constantNames;

std::vector<char> operators;
std::vector<char> leftBrackets;
std::vector<char> rightBrackets;

void initRef() {
	// functions
	unary_functions = {
		{ "sin", Func(std::sin) },
		{ "cos", Func(std::cos) },
		{ "tan", Func(std::tan) },
		{ "asin", Func(std::asin) },
		{ "acos", Func(std::acos) },
		{ "atan", Func(std::atan) },
		{ "csc", Func([](double x) -> double { return 1 / std::sin(x); }) },
		{ "sec", Func([](double x) -> double { return 1 / std::cos(x); }) },
		{ "cot", Func([](double x) -> double { return 1 / std::tan(x); }) },
		{ "acsc", Func([](double x) -> double { return std::asin(1 / x); }) },
		{ "asec", Func([](double x) -> double { return std::acos(1 / x); }) },
		{ "acot", Func([](double x) -> double { return std::atan(1 / x); }) },
		{ "ln", Func(std::log) },
		{ "log", Func(std::log10) },
		{ "sqrt", Func(std::sqrt) },
		{ "abs", Func(std::abs) }
	};

	binary_functions = {
		{ "+", Func([](double x, double y) -> double { return x + y; }, FuncTypes::OP, 2) },
		{ "-", Func([](double x, double y) -> double { return x - y; }, FuncTypes::OP, 2) },
		{ "*", Func([](double x, double y) -> double { return x * y; }, FuncTypes::OP, 3) },
		{ "/", Func([](double x, double y) -> double { return x / y; }, FuncTypes::OP, 3) },
		{ "%", Func(std::fmod, FuncTypes::OP, 3) },
		{ "^", Func([](double x, double y) -> double { return std::pow(x, y); }, FuncTypes::OP, 4, false) },
		{ "max", Func(std::fmax) },
		{ "min", Func(std::fmin) }
	};

	// function names
	functionNames = keys<Func>(unary_functions);
	std::vector<std::string> namesSet2 = keys<Func>(binary_functions);
	functionNames.insert(functionNames.end(), namesSet2.begin(), namesSet2.end());

	// constants
	constants = {
		{ "pi", std::atan(1) * 4 },
		{ "e", std::exp(1) }
	};

	// constant names
	constantNames = keys<double>(constants);

	// operator names
	operators.insert(operators.end(), { '+', '-', '/', '*', '%', '^' });

	// brackets
	leftBrackets.insert(leftBrackets.end(), { '(', '{', '[' });
	rightBrackets.insert(rightBrackets.end(), { ']', '}', ')' });
}

bool isNumber(char c) {
	return c == '.' || (int(c) >= int('0') && int(c) <= int('9'));
}

bool isNumber(const char* c) {
	return contains<std::string>(constantNames, c) || strcmp(c, ".") == 0 || containsNumbers(c);
}

double getNumericalVal(const char* c) {
	if (contains<std::string>(constantNames, c)) {
		return constants[c];
	} else {
		return std::atof(c);
	}
}

bool isFunction(std::string c) {
	return contains<std::string>(functionNames, c);
}

bool isLeftAssociative(std::string c) {
	return binary_functions[c].left;
}

short getPrecedence(std::string c) {
	if (contains<std::string>(keys(binary_functions), c)) {
		return binary_functions[c].prec;
	}

	return 0;
}

std::string findElement(int i, const char* eqn, std::vector<std::string> list) {
	for (std::string item : list) {
		int n = (int)item.size();
		if (std::string(eqn).substr(i, n) == item) {
			return item;
		}
	}
	
	return "";
}
