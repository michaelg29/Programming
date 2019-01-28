#pragma once

#include "../TcpNetworking.h"

#include <sstream>
#include <map>
#include <string>

#ifndef SERVERMESSAGE_H
#define SERVERMESSAGE_H

struct TCPNETWORKING_API ServerMessage {
	std::map<std::string, std::string> attributes;

	std::string toString();
	bool parse(std::string msg);
};

#endif // !SERVERMESSAGE_H