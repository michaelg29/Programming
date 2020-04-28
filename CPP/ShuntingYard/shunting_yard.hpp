#ifndef SHUNTING_YARD
#define SHUNTING_YARD

#include <vector>
#include <string>
#include <map>
#include <stack>

namespace ShuntingYard {
	/*
		Typedefs
	*/

	// reverse polish notation list
	typedef std::vector<std::string> RPN;

	// evaluate unary function (1 argument)
	typedef double(*UnaryFuncEval)(double x);

	// evaluate binary function (2 arguments)
	typedef double(*BinaryFuncEval)(double x, double y);

	// types
	enum class TokenTypes {
		OPERATOR,
		CONSTANT,
		FUNCTION,
		LPAREN,
		RPAREN,
		ELSE
	};

	/*
		Utility function callbacks
	*/

	// vector contains value
	template <typename T>
	bool contains(std::vector<T> v, T x);

	// obtain key list
	template <typename T>
	std::vector<std::string> keys(std::map<std::string, T> m);

	// obtain combined key list
	template <typename T>
	std::vector<std::string> keys(std::map<std::string, T> m1, std::map<std::string, T> m2);

	// determine if character is number
	bool isNumber(char c);

	// determine if entire string is number
	bool isNumber(const char* c);

	// determine if string only contains numerical characters
	bool containsNumbers(const char* str);

	// get numerical value of string
	double getNumericalVal(const char* c);

	// determine if string matches a function
	bool isFunction(std::string c);

	// determine if a function is left associative
	bool isLeftAssociative(std::string c);

	// get the function precedence
	short getPrecedence(std::string c);

	// find element from list in eqn starting at index i
	std::string findElement(int i, const char* eqn, std::vector<std::string> list);

	/*
		Function Class definition
	*/
	class Func {
	public:
		Func()
			: type(TokenTypes::OPERATOR), prec(0), left(true), unary(true), u_eval(nullptr), b_eval(nullptr) {}

		Func(UnaryFuncEval eval, TokenTypes type = TokenTypes::FUNCTION, short prec = 0, bool left = true)
			: Func(type, prec, left) {
			u_eval = eval;
			unary = true;
		}

		Func(BinaryFuncEval eval, TokenTypes type = TokenTypes::FUNCTION, short prec = 0, bool left = true)
			: Func(type, prec, left) {
			b_eval = eval;
			unary = false;
		}

		double eval(double x, double y = 0) {
			return (this->unary) ? u_eval(x) : b_eval(x, y);
		}

		UnaryFuncEval u_eval;
		BinaryFuncEval b_eval;

		TokenTypes type;
		short prec;
		bool left;
		bool unary;

	private:
		Func(TokenTypes type, short prec, bool left)
			: type(type), prec(prec), left(left), unary(true), u_eval(nullptr), b_eval(nullptr) {}
	};

	/*
		Reference
	*/

	// unary functions
	std::map<std::string, Func> unary_functions = {
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
	// binary functions
	std::map<std::string, Func> binary_functions = {
		{ "+", Func([](double x, double y) -> double { return x + y; }, TokenTypes::OPERATOR, 2) },
		{ "-", Func([](double x, double y) -> double { return x - y; }, TokenTypes::OPERATOR, 2) },
		{ "*", Func([](double x, double y) -> double { return x * y; }, TokenTypes::OPERATOR, 3) },
		{ "/", Func([](double x, double y) -> double { return x / y; }, TokenTypes::OPERATOR, 3) },
		{ "%", Func(std::fmod, TokenTypes::OPERATOR, 3) },
		{ "^", Func([](double x, double y) -> double { return std::pow(x, y); }, TokenTypes::OPERATOR, 4, false) }
	};

	// function names
	std::vector<std::string> functionNames = keys<Func>(unary_functions, binary_functions);

	// constants
	std::map<std::string, double> constants = {
		{ "pi", std::atan(1) * 4 },
		{ "e", std::exp(1) }
	};
	// constant names
	std::vector<std::string> constantNames = keys<double>(constants);

	// variables
	std::map<std::string, double> variables;

	// operators
	std::vector<char> operators = { '+', '-', '/', '*', '%', '^' };
	// left brackets
	std::vector<char> leftBrackets = { '(', '{', '[' };
	// right brackets
	std::vector<char> rightBrackets = { ']', '}', ')' };

	/*
		Node class definitions
	*/

	// base node class
	class Node {
	public:
		Node(std::string name, bool isFunc)
			: name(name), isFunc(isFunc) {}

		double eval(double x = 0, double y = 0);

		std::string name;
		bool isFunc;

		Node* right;
		Node* left;
	};

	// function node class
	class FuncNode : public Node {
	public:
		FuncNode(std::string name)
			: Node(name, true) {}

		// set type of function and then assign that callback
		void setUnary(bool isUnary) {
			this->isUnary = isUnary;

			if (isUnary) {
				this->func = unary_functions[name];
			}
			else {
				this->func = binary_functions[name];
			}
		}

		// evaluate the member function
		double eval(double x, double y = 0) {
			return this->func.eval(x, y);
		}

		Func func;
		bool isUnary;
	};

	// number node class
	class NumNode : public Node {
	public:
		NumNode(std::string name)
			: Node(name, false) {}

		// return numerical value of function
		double eval(double x = 0, double y = 0) {
			return getNumericalVal(name.c_str());
		}
	};

	/*
		Main functions
	*/

	// parse into reverse polish notation
	RPN reversePolishNotation(const char* eqn) {
		std::vector<std::string> queue;
		std::stack<std::string> stack;

		std::string obj = "";
		TokenTypes type = TokenTypes::ELSE;
		TokenTypes prevType = TokenTypes::ELSE; // negative sign detection

		for (int i = 0, eq_len = (int)strlen(eqn); i < eq_len; i++) {
			char t = eqn[i];

			// skip spaces/commas
			if (t == ' ' || t == ',') {
				prevType = TokenTypes::ELSE;
				continue;
			}

			// classify token
			if (isNumber(t)) {
				type = TokenTypes::CONSTANT;

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

				if (obj == "-" && prevType != TokenTypes::RPAREN && prevType != TokenTypes::OPERATOR) {
					// interpret as subtraction sign
					type = TokenTypes::OPERATOR;
				}
			}
			else {
				obj = findElement(i, eqn, functionNames);
				if (obj != "") {
					type = contains<char>(operators, obj[0]) ? TokenTypes::OPERATOR : TokenTypes::FUNCTION;
				}
				else {
					obj = findElement(i, eqn, constantNames);
					if (obj != "") {
						type = TokenTypes::CONSTANT;
					}
					else {
						obj = findElement(i, eqn, keys<double>(variables));
						if (obj != "") {
							type = TokenTypes::CONSTANT;
						}
						else {
							if (contains<char>(leftBrackets, t)) {
								type = TokenTypes::LPAREN;
								obj = "(";
							}
							else if (contains<char>(rightBrackets, t)) {
								type = TokenTypes::RPAREN;
								obj = ")";
							}
							else {
								type = TokenTypes::ELSE;
							}
						}
					}
				}
				i += obj.size() - 1;
			}

			// do something with token
 			const char* last_stack = (stack.size() > 0) ? stack.top().c_str() : "";
			switch (type) {
			case TokenTypes::CONSTANT:
				queue.push_back(obj);
				break;
			case TokenTypes::FUNCTION:
				stack.push(obj);
				break;
			case TokenTypes::OPERATOR:
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
			case TokenTypes::LPAREN:
				stack.push("(");
				break;
			case TokenTypes::RPAREN:
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

			prevType = type;
		}

		while (stack.size() > 0) {
			queue.push_back(stack.top());
			stack.pop();
		}

		return queue;
	}

	// parse RPN to tree
	Node* parse(RPN rpn) {
		std::stack<Node*> stack;

		for (std::string item : rpn) {
			if (isNumber(item.c_str())) {
				stack.push(new NumNode(item));
			}
			else {
				FuncNode* ftr = new FuncNode(item);
				if (contains<std::string>(keys(binary_functions), item)) {
					ftr->setUnary(false);
					ftr->right = stack.top();
					stack.pop();
					ftr->left = stack.top();
					stack.pop();
				}
				else if (contains<std::string>(keys(unary_functions), item)) {
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

	// evaluate tree
	double eval(Node* tree) {
		if (tree->isFunc) {
			FuncNode* ftree = (FuncNode*)tree;
			if (ftree->func.unary) {
				return ftree->eval(eval(tree->left), 0);
			}
			else {
				return ftree->eval(eval(tree->left), eval(tree->right));
			}
		}
		else {
			NumNode* ntree = (NumNode*)tree;
			return ntree->eval();
		}
	}

	/*
		Utility function definitions
	*/

	// vector contains value
	template <typename T>
	bool contains(std::vector<T> v, T x) {
		return std::find(v.begin(), v.end(), x) != v.end();
	}

	// obtain key list
	template <typename T>
	std::vector<std::string> keys(std::map<std::string, T> m) {
		std::vector<std::string> ret;

		// push each key from each iterated pair in map
		for (auto const& element : m) {
			ret.push_back(element.first);
		}

		return ret;
	}

	// obtain combined key list
	template <typename T>
	std::vector<std::string> keys(std::map<std::string, T> m1, std::map<std::string, T> m2) {
		// get keys from each map
		std::vector<std::string> keySet1 = keys<T>(m1);
		std::vector<std::string> keySet2 = keys<T>(m2);

		// insert second list onto end of first
		keySet1.insert(keySet1.end(), keySet2.begin(), keySet2.end());

		// return result
		return keySet1;
	}

	// determine if character is number (either decimal point or has matching key value)
	bool isNumber(char c) {
		return c == '.' || (int(c) >= int('0') && int(c) <= int('9')) || c == '-';
	}

	// determine if entire string is number (constant, decimal point, or contains only numbers
	bool isNumber(const char* c) {
		return contains<std::string>(constantNames, c) || contains<std::string>(keys<double>(variables), c) || strcmp(c, ".") == 0 || containsNumbers(c);
	}

	// determine if string only contains numerical characters
	bool containsNumbers(const char* str) {
		// try to prove wrong
		/*
			either not a decimal point
				or wrong key value (outside of 0-9)
		*/
		for (char c : std::string(str)) {
			if (!isNumber(c)) {
				return false;
			}
		}

		return true;
	}

	// get numerical value of string
	double getNumericalVal(const char* c) {
		if (contains<std::string>(constantNames, c)) {
			return constants[c];
		}
		else if (contains<std::string>(keys<double>(variables), c)) {
			return variables[c];
		}
		else {
			return std::atof(c);
		}
	}

	// determine if string matches a function
	bool isFunction(std::string c) {
		return contains<std::string>(functionNames, c);
	}

	// determine if a function is left associative
	bool isLeftAssociative(std::string c) {
		return binary_functions[c].left;
	}

	// get the function precedence
	short getPrecedence(std::string c) {
		if (contains<std::string>(keys(binary_functions), c)) {
			return binary_functions[c].prec;
		}

		return 0;
	}

	// find element from list in eqn starting at index i
	std::string findElement(int i, const char* eqn, std::vector<std::string> list) {
		for (std::string item : list) {
			int n = (int)item.size();
			if (std::string(eqn).substr(i, n) == item) {
				return item;
			}
		}

		return "";
	}
}

#endif