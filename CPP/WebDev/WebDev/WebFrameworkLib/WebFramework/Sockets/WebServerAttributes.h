#ifndef WEBSERVERATTRIBUTES
#define WEBSERVERATTRIBUTES

#include "../WebFramework.h"

#include <string>
#include <map>

// method template for view
typedef void(*View)(Request request);

struct WEBFRAMEWORK_API WebServerAttributes {
public:
	std::string contextRoute;
	std::string defaultFile;
	std::string errorFile;
	std::string templateRoute;
	std::string stylesheetRoute;

	std::map<std::string, View> routes;

	View getView(std::string route);

	WebServerAttributes() { }
};

#endif