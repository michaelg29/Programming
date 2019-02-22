#pragma once

#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "../WebFramework.h"

#include "TcpListener.h"
#include "WebClient.h"
#include "WebServerAttributes.h"
#include "../Requests/Request.h"

#include <vector>
#include <string>

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