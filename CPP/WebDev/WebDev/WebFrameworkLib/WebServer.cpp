#include "WebFramework/Sockets/WebServer.h"

WebServer::WebServer(const char* ipAddress, int port) :
	TcpListener(ipAddress, port) 
{ 
	attributes.contextRoute = "wwwroot";
	attributes.defaultFile = "index.html";
	attributes.errorFile = "error.html";
	attributes.templateRoute = "templates";
	attributes.routes = {};
}

void WebServer::onClientConnected(int clientSocket) {
	std::map<std::string, std::string> context;
	clients.push_back(WebClient(clientSocket));
}

void WebServer::onClientDisconnected(int clientSocket) {

}

void WebServer::onMessageReceived(int clientSocket, const char* msg, int length) {
	for (WebClient client : clients) {
		if (client.socket == clientSocket) {
			Request request(client, std::string(msg), attributes);
			request.parse();
			View view = attributes.getView(request.route);
			if (view == nullptr) {
				request.readFile(attributes.contextRoute + '/' + attributes.errorFile);
				request.forward();
			}
			else {
				view(request);
			}
		}
	}
}
