#include <string>

#ifndef FUNC
#define FUNC

// Callback to function evaluation
typedef double(*UnaryFuncEval)(double x);
typedef double(*BinaryFuncEval)(double x, double y);

class FuncTypes {
public:
	enum Types {
		OP = 'o',
		CONST = 'c',
		FUNCTION = 'f',
		ELSE = 'e',
		LPAREN = '(',
		RPAREN = ')'
	};
};

class Func {
public:
	Func();

	Func(UnaryFuncEval eval, char type = FuncTypes::FUNCTION, short prec = 0, bool left = true);

	Func(BinaryFuncEval eval, char type = FuncTypes::FUNCTION, short prec = 0, bool left = true);

	double eval(double x, double y=0);

	UnaryFuncEval u_eval;
	BinaryFuncEval b_eval;

	char type;
	short prec;
	bool left;
	bool unary;

private:
	Func(char type, short prec, bool left);
};

#endif