#pragma once

#ifndef WEBCLIENT_H
#define WEBCLIENT_H

#include "../WebFramework.h"

#include <map>

struct WebClient {
public:
	int socket;
	std::map<std::string, std::string> context;		// context values to be accessed in the web pages

	WebClient(int socket) :
		socket(socket) { }
};

#endif