#include <fstream>
#include <sstream>

#include <iostream>

#include "json.h"
using namespace json;

json_data json_util::read(const char* filename) {
	std::string content = json_util::readFile(filename);

	return json_data::parse(content);
}

std::string json_util::readFile(const char* filename) {
	std::ifstream file(filename);
	std::string ret = "";

	if (file.is_open()) {
		std::stringstream ss;
		ss << file.rdbuf();
		ret = ss.str();
	}

	file.close();
	return ret;
}

bool json_util::write(const char* filename, json_data val) {
	return json_util::writeFile(filename, val.stringify());
}

bool json_util::writeFile(const char* filename, std::string content) {
	std::cout << content << std::endl;
	std::ofstream file(filename);
	
	if (file.is_open()) {
		file << content;
		file.close();
		return true;
	}

	return false;
}