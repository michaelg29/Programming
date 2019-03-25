#include <string>

#ifndef PROGRAM
#define PROGRAM

class Program {
public:
	Program(std::string name);

private:

protected:
	void doAction();

	std::string name;
};

#endif