#include "util.h"

bool json::isNum(char c) {
	std::string possibleChars = "1234567890.";

	return possibleChars.find(c) != std::string::npos;
}