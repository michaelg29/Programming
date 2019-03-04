#include <Sockets/WebServer.h>

#include <iostream>

#define IP_ADDRESS ("127.0.0.1")
#define PORT 8080

void home(Request request);

int main() {
	WebServer webserver(IP_ADDRESS, PORT);

	webserver.attributes.routes = {
		{"", home},
		{"home", home},
	};

	webserver.attributes.contextRoute = "wwwroot";		// folder name containing html files next to executable
	webserver.attributes.defaultFile = "index.html";	// file linked to request of "/"
	webserver.attributes.errorFile = "error.html";		// file displayed when error code 404
	webserver.attributes.templateRoute = "templates";	// template folder in contextRoute folder
	webserver.attributes.stylesheetRoute = "style";		// style folder in contextRoute folder

	if (webserver.init() != 0)
		return -1;

	webserver.run();

	return 0;
}

void home(Request request) {
	for (std::pair<std::string, std::string> param : request.params) {
		request.setContext(param.first, param.second);
	}

	if (request.method == "POST") {
		request.setContext("test", request.data["test"]);
		request.readFile("wwwroot/foo.html");
	}
	else
		request.readFile("wwwroot/index.html");
	
	request.forward();
}
