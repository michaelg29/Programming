#include "json.h"
#include "util.h"

#include <sstream>
#include <iostream>

using namespace json;

std::string json_bool::TRUE = "true";
std::string json_bool::FALSE = "false";

json_data json_data::parse(std::string str) {
	std::string openDelimeters = "[{";
	std::string closeDelimeters = "]}";

	int idx1 = 0;
	int openCount = 0;
	bool quoteOpen = false;

	// identify type
	char c = str[0];

	if (isNum(c)) {
		// number
		return json_num(std::atof(str.c_str()));
	}
	else if (firstAndLastMatch(str, '"', '"') || firstAndLastMatch(str, '\'', '\'')) {
		// string
		return json_string(str.substr(1, str.length() - 2));
	}
	else if (firstAndLastMatch(str, '[', ']')) {
		// list of data
		std::vector<json_data> ret;

		std::string val;

		bool itemOpen = false;

		for (int i = 1; i < str.length() - 1; i++) {
			c = str[i];

			if (isEmpty(c)) {
				// ignore if whitespace
				continue;
			}

			if (c == '"' && str[i - 1] != '\\') {
				openCount += quoteOpen ? -1 : 1;

				if (!quoteOpen) {
					idx1 = i;
				}

				quoteOpen = !quoteOpen;
			}

			if (!quoteOpen && str[i - 1] != '\\') {
				if (strContains(openDelimeters, c)) {
					// open token
					openCount++;
				}
				else if (strContains(closeDelimeters, c)) {
					// close token
					openCount--;
				}
			}

			if (openCount == 0 && val != "" && c == ',') {
				ret.push_back(json_data::parse(val));
				val = "";
			}
			else {
				val += c;
			}
		}

		ret.push_back(json_data::parse(val));

		return json_list(ret);
	}
	else if (firstAndLastMatch(str, '{', '}')) {
		// object (list of pairs)
		int i = 1;

		std::string key = "";;
		json_data value;

		std::map<std::string, json_data> ret;

		while (i < str.length()) {
			// get key
			c = str[i];

			// get first character
			while (isEmpty(c) && i < str.length()) {
				c = str[++i];
			}

			if (c != '"') {
				break;
			}

			c = str[++i];

			// find end of string
			while (c != '"' && str[i - 1] != '\\') {
				key += c;
				c = str[++i];
			}

			c = str[++i];

			// skip to item
			while (isEmpty(c) || c == ':') {
				c = str[++i];
			}

			bool itemOpen = false;
			std::string val = "";

			// get item
			for (; i < str.length(); i++) {
				c = str[i];

				if (isEmpty(c)) {
					// ignore if whitespace
					continue;
				}

				if (c == '"' && str[i - 1] != '\\') {
					openCount += quoteOpen ? -1 : 1;

					if (!quoteOpen) {
						idx1 = i;
					}

					quoteOpen = !quoteOpen;
				}

				if (!quoteOpen && str[i - 1] != '\\') {
					if (strContains(openDelimeters, c)) {
						// open token
						openCount++;
					}
					else if (strContains(closeDelimeters, c)) {
						// close token
						openCount--;
					}
				}

				if (openCount <= 0 && val != "" && (c == ',' || c == '}')) {
					value = json_data::parse(val);
					i++;
					break;
				}
				else {
					val += c;
				}
			}

			// insert/reset
			ret.insert(std::pair<std::string, json_data>(key, value));
			key = "";
			value = json_data();
		}



		return json_object(ret);
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
	ret = ret.substr(0, ret.length() - 2);
	
	return '[' + ret + ']';
}

std::string json_object::stringify() {
	std::stringstream ret_ss;

	for (std::pair<std::string, json_data> pair : o_val) {
		ret_ss << '"' << pair.first << "\": " << pair.second.stringify() << ',' << std::endl;
	}

	std::string ret = ret_ss.str();
	ret = ret.substr(0, ret.length() - 2);

	return '{' + ret + '}';
}