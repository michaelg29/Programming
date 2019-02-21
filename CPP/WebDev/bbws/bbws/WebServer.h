#pragma once

#include "TcpListener.h"

#include <map>

class WebServer : public TcpListener {
public:

	WebServer(const char* ipAddress, int port) :
		TcpListener(ipAddress, port) { }

protected:

	// handler for client connections
	virtual void onClientConnected(int clientSocket);

	// handler for client disconnections
	virtual void onClientDisconnected(int clientSocket);

	// handler for message from client
	virtual void onMessageReceived(int clientSocket, const char* msg, int length);

private:
	std::map<std::string, std::string> context;

};