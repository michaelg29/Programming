#include "ServerMessage.h"

std::string ServerMessage::toString() {
	std::ostringstream ret;

	ret << "{";

	for (std::pair<std::string, std::string> attribute : attributes) {
		ret << attribute.first << ':' << attribute.second << ';';
	}

	ret << "}";

	return ret.str();
}

bool ServerMessage::parse(std::string msg) {
	int start = 0, end = 0;
	std::string key, value;
	bool addToKey = true;

	if (msg[0] != '{' || msg[sizeof(msg) / sizeof(msg[0]) - 1] != '}')
		return false;

	std::string::const_iterator it;

	for (it = msg.begin(); it != msg.end(); it++) {
		if (*it == '{') {
			continue;
		}
		else if (*it == '}') {
			break;
		}
		else if (*it == ':') {
			addToKey = false;
		}
		else if (*it == ';') {
			addToKey = true;
			attributes.insert(key, value);
			key = "";
			value = "";
		}
		else {
			if (addToKey)
				key += *it;
			else
				value += *it;
		}
	}

	return true;
}