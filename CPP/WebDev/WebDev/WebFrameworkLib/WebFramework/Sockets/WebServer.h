#pragma once

#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "../WebFramework.h"

#include "TcpListener.h"
#include "WebClient.h"

#include <vector>
#include <string>

struct WebServerAttributes {
	std::string contextRoute;
	std::string defaultFile;
	std::string errorFile;

	std::map<std::string, std::string> routes;

	std::string getFilePath(std::string route);
};

class WebServer : public TcpListener {
public:
	WebServer(const char* ipAddress, int port);

	std::vector<WebClient> clients;		// client list
	WebServerAttributes attributes;		// attributes structure (file defaults, routes)

protected:

	// handler for client connections
	virtual void onClientConnected(int clientSocket);

	// handler for client disconnections
	virtual void onClientDisconnected(int clientSocket);

	// handler for message from client
	virtual void onMessageReceived(int clientSocket, const char* msg, int length);
};

#endif