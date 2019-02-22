#pragma once

#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "TcpListener.h"
#include "WebClient.h"

#include <vector>
#include <string>

class WebServer : public TcpListener {
public:

	WebServer(const char* ipAddress, int port) :
		TcpListener(ipAddress, port) { }

	std::map<std::string, std::string> context;		// context values to be accessed in the web pages
	std::vector<WebClient> clients;

protected:

	// handler for client connections
	virtual void onClientConnected(int clientSocket);

	// handler for client disconnections
	virtual void onClientDisconnected(int clientSocket);

	// handler for message from client
	virtual void onMessageReceived(int clientSocket, const char* msg, int length);
};

#endif