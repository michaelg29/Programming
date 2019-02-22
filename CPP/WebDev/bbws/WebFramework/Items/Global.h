#pragma once

#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>
#include <map>

enum Method { GET, HEAD, POST, PUT, CONNECT, OPTIONS, TRACE };

class Global {
public:
	static std::string contextRoute;					// base folder for html files
	static std::string defaultFile;						// default file
	static std::string errorFile;						// error file
	static std::map<std::string, std::string> routes;	// route values for views
	static std::map<std::string, Method> stoMethod;

	static std::string getFilePath(std::string route);
	
};

namespace std {
	void replace_all(std::string &str, const std::string &from, const std::string &to);
}

#endif