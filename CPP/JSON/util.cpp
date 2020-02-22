#include "util.h"

bool json::isNum(char c) {
	return json::strContains("0123456789.", c);
}

bool json::firstAndLastMatch(std::string str, char first, char last) {
	return str[0] == first && str[str.length() - 1] == last;
}

bool json::strContains(std::string str, char c) {
	return str.find(c) != std::string::npos;
}

bool json::isEmpty(char c) {
	return c == ' ' || c == '\n' || c == '\t';
}