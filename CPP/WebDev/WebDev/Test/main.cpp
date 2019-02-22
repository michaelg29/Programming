#include <Sockets/WebServer.h>

#define IP_ADDRESS ("127.0.0.1")
#define PORT 8080

void home(Request request);

int main() {
	WebServer webserver(IP_ADDRESS, PORT);

	webserver.attributes.routes = {
		{"", home},
		{"home", home}
	};

	webserver.attributes.contextRoute = "wwwroot";		// folder name containing html files next to executable
	webserver.attributes.defaultFile = "index.html";	// file linked to request of "/"
	webserver.attributes.errorFile = "error.html";		// file displayed when error code 404

	if (webserver.init() != 0)
		return -1;

	webserver.run();

	return 0;
}

void home(Request request) {
	request.readFile("wwwroot/index.html");
	request.forward();
}