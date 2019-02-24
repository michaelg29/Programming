#ifndef WEBCLIENT
#define WEBCLIENT

#include "../WebFramework.h"

#include <map>

struct WEBFRAMEWORK_API WebClient {
public:
	int socket;
	std::map<std::string, std::string> context;		// context values to be accessed in the web pages

	WebClient(int socket) :
		socket(socket) { }
};

#endif