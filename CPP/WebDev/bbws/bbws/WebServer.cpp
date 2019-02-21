#include <string>
#include <iostream>
#include <sstream> 
#include "WebServer.h"

void WebServer::onClientConnected(int clientSocket) {

}

void WebServer::onClientDisconnected(int clientSocket) {

}

void WebServer::onMessageReceived(int clientSocket, const char* msg, int length) {
	// GET /index.html HTTP/1.1

	// parse document requested

	// open document in local file system

	// write the document back to the client
	std::ostringstream oss;
	oss << "HTTP/1.1 200 OK\r\n";
	oss << "Cache-Control: no-cache, private\r\n";
	oss << "Content-Type: text/html\r\n";
	oss << "Content-Length: 14\r\n";
	oss << "\r\n";
	oss << "<h1>Hello</h1>";

	std::string output = oss.str();
	int size = output.size() + 1;

	sendToClient(clientSocket, output.c_str(), size);
}