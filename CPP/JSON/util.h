#ifndef UTIL_H
#define UTIL_H

#include <string>

namespace json {
	bool isNum(char c);
	bool firstAndLastMatch(std::string str, char first, char last);
	bool strContains(std::string str, char c);
	bool isEmpty(char c);
}

#endif