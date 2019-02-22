#pragma once

#ifndef WEBSERVERATTRIBUTES_H
#define WEBSERVERATTRIBUTES_H

#include "../WebFramework.h"

#include <string>
#include <map>

// method template for view
typedef void(*View)(Request request);

struct WebServerAttributes {
public:
	std::string contextRoute;
	std::string defaultFile;
	std::string errorFile;

	std::map<std::string, View> routes;

	View getView(std::string route);

	WebServerAttributes() { }
};

#endif