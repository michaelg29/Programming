#include <map>
#include <vector>
#include "func.h"

#ifndef REF
#define REF

/*
dictionaries/lists
*/

// functions with one input
extern std::map<std::string, Func> unary_functions;

// functions with two inputs
extern std::map<std::string, Func> binary_functions;

// function names
extern std::vector<std::string> functionNames;

// constants
extern std::map<std::string, double> constants;

// constant names
extern std::vector<std::string> constantNames;

// operators
extern std::vector<char> operators;

// brackets
extern std::vector<char> leftBrackets;
extern std::vector<char> rightBrackets;

bool isNumber(char c);
bool isNumber(const char* c);
double getNumericalVal(const char* c);
bool isFunction(std::string c);
bool isLeftAssociative(std::string c);

short getPrecedence(std::string c);
std::string findElement(int i, const char* eqn, std::vector<std::string> list);

// initialize lists/dictionaries
void initRef();

#endif