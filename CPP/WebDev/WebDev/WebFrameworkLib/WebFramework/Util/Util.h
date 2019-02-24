#ifndef UTIL
#define UTIL

#include "../WebFramework.h"
#include "../Sockets/WebServer.h"
#include "../Requests/Request.h"

#include <string>
#include <map>

namespace WebServerUtil {
	void replace_all(std::string &str, const std::string &from, const std::string &to);

	namespace FileParser {
		std::string parseFile(std::string path, Request *request);
		void addContext(std::string &content, Request *request);
		void insertTemplates(std::string &content, Request *request);
	}
}

#endif