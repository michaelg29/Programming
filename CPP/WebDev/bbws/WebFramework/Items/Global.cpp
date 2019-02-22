#include "Global.h"

std::string Global::contextRoute = "wwwroot";
std::string Global::defaultFile = "index.html";
std::string Global::errorFile = "error.html";

std::map<std::string, std::string> Global::routes = {
	{"", "index.html"},
	{"foo", "foo.html"}
};

std::map<std::string, Method> Global::stoMethod = {
	{"GET", GET},
	{"HEAD", HEAD},
	{"POST", POST},
	{"PUT", PUT},
	{"CONNECT", CONNECT},
	{"OPTIONS", OPTIONS},
	{"TRACE", TRACE}
};

std::string Global::getFilePath(std::string route) {
	for (std::pair<std::string, std::string> pair : routes) {
		if (route == "/" + pair.first) {
			return contextRoute + "/" + pair.second;
		}
	}

	return contextRoute + "/" + errorFile;
}

void std::replace_all(std::string &str, const std::string &from, const std::string &to) {
	int pos = 0;
	int flen = from.length();
	while ((pos = str.find(from, pos)) != -1) {
		str.replace(pos, flen, to);
		pos += flen;
	}
}