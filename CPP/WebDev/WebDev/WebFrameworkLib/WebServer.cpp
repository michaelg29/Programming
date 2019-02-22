#include "WebFramework/Sockets/WebServer.h"
#include "WebFramework/Requests/Request.h"

std::string WebServerAttributes::getFilePath(std::string route) {
	for (std::pair<std::string, std::string> pair : routes) {
		if (route == "/" + pair.first) {
			return contextRoute + "/" + pair.second;
		}
	}

	return contextRoute + "/" + errorFile;
}

WebServer::WebServer(const char* ipAddress, int port) :
	TcpListener(ipAddress, port) { 

	attributes.contextRoute = "wwwroot";
	attributes.defaultFile = "index.html";
	attributes.errorFile = "error.html";
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
			Request request = Request(client, msg, attributes);
			std::string response = request.parse();

			int size = response.size() + 1;

			sendToClient(clientSocket, response.c_str(), size);
		}
	}
}
