#include <fstream>
#include <sstream>

#include "json.h"
using namespace json;

json_data json_util::parse(std::string json_str) {
	return json_data::parse(json_str);
}

json_data json_util::read(const char* filename) {
	std::string content = json_util::readFile(filename);

	return json_util::parse(content);
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

std::string json_util::stringify(json_data data) {
	std::string ret = "";

	return ret;
}

bool json_util::write(const char* filename, json_data val) {
	return json_util::writeFile(filename, json_util::stringify(val));
}

bool json_util::writeFile(const char* filename, std::string content) {
	std::ofstream file(filename);
	
	if (file.is_open()) {
		file << content;
		file.close();
		return true;
	}

	return false;
}