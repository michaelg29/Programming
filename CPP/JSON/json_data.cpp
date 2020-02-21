#include "json.h"
#include "util.h"

#include <sstream>

using namespace json;

std::string json_bool::TRUE = "true";
std::string json_bool::FALSE = "false";

json_data json_data::parse(std::string str) {
	// identify type
	char c = str[0];

	if (isNum(c)) {
		// number
		return json_num(std::atof(str.c_str()));
	}
	else if (c == '"' || c == '\'') {
		// string
		return json_string(str.substr(1, str.length() - 2));
	}
	else if (c == '[') {
		// list
		std::string openDelimeters = "[{\"'";
		std::string closeDelimeters = "]}\"'";
	}
	else if (c == '{') {
		// object
	}
	else if (c == 't') {
		// possibly true
		if (str.substr(0, json_bool::TRUE.length()) == json_bool::TRUE) {
			// is true
			return json_bool(true);
		}
	}
	else if (c == 'f') {
		// possibly false
		if (str.substr(0, json_bool::FALSE.length()) == json_bool::FALSE) {
			// is false
			return json_bool(false);
		}
	}

	return json_data();
}

std::string json_data::stringify() {
	switch (type) {
	case json_type::json_string:
		return json_string(s_val).stringify();
	case json_type::json_num:
		return json_num(n_val).stringify();
	case json_type::json_bool:
		return json_bool(b_val).stringify();
	case json_type::json_list:
		return json_list(l_val).stringify();
	case json_type::json_object:
		return json_object(o_val).stringify();
	default:
		return "";
	}
}

std::string json_data::getType() {
	switch (type) {
	case json_type::json_string:
		return "string";
	case json_type::json_num:
		return "number";
	case json_type::json_bool:
		return "bool";
	case json_type::json_list:
		return "list";
	case json_type::json_object:
		return "json object";
	default:
		return "";
	}
}

std::string json_string::stringify() {
	std::stringstream ret;

	ret << '"' << s_val << '"';

	return ret.str();
}

std::string json_num::stringify() {
	std::stringstream ret;

	ret << n_val;

	return ret.str();
}

std::string json_bool::stringify() {
	return b_val ? "true" : "false";
}

std::string json_list::stringify() {
	std::stringstream ret_ss;

	for (json_data data : l_val) {
		ret_ss << data.stringify() << ',' << std::endl;
	}

	std::string ret = ret_ss.str();
	ret.pop_back();

	return ret;
}

std::string json_object::stringify() {
	std::stringstream ret_ss;

	for (std::pair<std::string, json_data> pair : o_val) {
		ret_ss << '"' << pair.first << "\": " << pair.second.stringify() << ',' << std::endl;
	}

	std::string ret = ret_ss.str();
	ret.pop_back();

	return ret;
}