#include "func.h"

Func::Func()
	: type(FuncTypes::OP), prec(0), left(true), unary(true), u_eval(nullptr), b_eval(nullptr) {}

Func::Func(char type, short prec, bool left)
	: type(type), prec(prec), left(left), unary(true), u_eval(nullptr), b_eval(nullptr) {}

Func::Func(UnaryFuncEval eval, char type, short prec, bool left)
	: Func(type, prec, left) {
	this->u_eval = eval;
	this->unary = true;
}

Func::Func(BinaryFuncEval eval, char type, short prec, bool left)
	: Func(type, prec, left) {
	this->b_eval = eval;
	this->unary = false;
}

double Func::eval(double x, double y) {
	return (this->unary) ? u_eval(x) : b_eval(x, y);
}