#pragma once

#include "../TcpNetworking.h"
#include "NetworkNode.h"

#include <vector>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

#ifndef TCPLISTENER_H
#define TCPLISTENER_H

#define MAX_BUFFER_SIZE (49152)

// Callback to data receieved method
typedef void(*MessageReceivedHandler)(TcpListener* listener, int socketId, std::string msg);
// Callback to server event method
typedef void(*ServerEventHandler)(TcpListener* listener, std::string msg);
// Callback to server command method
typedef void(*CommandHandler)(TcpListener* listener, int socket, std::string cmd);

class TCPNETWORKING_API TcpListener {
public:
	/**
		constructor for listener class, initializes all private variables, sets up server

		@param ipAddress to be stored in m_ipAddress
		@param port to be stored in m_port
		@param cmdIndicator to be stored in cmdIndicator
		@param msgHandler to be stored in msgHandler
		@param eventHandler to be stored in eventHandler
		@param cmdHandler to be stored in cmdHandler
	*/
	TcpListener(std::string ipAddress,
		int port,
		std::string cmdIndicator,
		MessageReceivedHandler msgHandler,
		ServerEventHandler eventHandler,
		CommandHandler cmdHandler);

	~TcpListener();

	// Initialize winsock
	int init();

	// main processing - receive communications loop
	void run();

	// drop client
	void disconnectClient(SOCKET client);

	// force disconnect server
	void disconnect();

	// cleanup
	void cleanup();

	// send message to a client, if client == -1, sends to all
	void send_msg(std::string msg, SOCKET client = -1);

	std::vector<NetworkNode> clients;	// vector storing client nodes

private:
	// Create server socket
	NetworkNode createSocket();

	// Get Connection
	NetworkNode getConnection();

	std::string				m_ipAddress;	// stores the ip address the socket is hosted on
	int						m_port;			// stores the port the socket listens on
	bool					m_running;		// boolean value representing state of server
	NetworkNode				m_node;			// stores node information for listener
	MessageReceivedHandler	msgHandler;		// points to function that handles received messages
	ServerEventHandler		eventHandler;	// points to function that handles server events
	CommandHandler			cmdHandler;		// points to function that handles server commands
	std::string				cmdIndicator;	// stores string value that indicates a command
	fd_set					master;			// stores set of connected clients
};

#endif // !TCPLISTENER_H