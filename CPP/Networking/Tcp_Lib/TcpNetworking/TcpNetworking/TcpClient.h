#pragma once

#include "../TcpNetworking.h"
#include <WS2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#ifndef TCPCLIENT_H
#define TCPCLIENT_H

// Callback to client event handler method
typedef void(*EventHandler)(TcpClient* client, std::string msg);

// Callback to data received method
typedef void(*MessageReceivedHandler)(TcpClient* client, std::string msg);

class TCPNETWORKING_API TcpClient
{
public:
	TcpClient(std::string ipAddress, int port, EventHandler eventHandler, MessageReceivedHandler msgHandler);
	~TcpClient();

	// Initialize winsock
	int Init();

	// Receive communications loop - main processing
	void Run();

	// Force disconnect the server
	void ForceDisconnect();

	// Cleanup
	void Cleanup();

	// Send a message to a server
	void Send(std::string msg);

	bool running;

private:
	// Create the client socket
	bool CreateSocket();

	// Connect to the socket at the ip address and port
	int Connect();

	std::string				m_ipAddress;
	int						m_port;
	sockaddr_in				m_hint;
	SOCKET					m_socket;
	EventHandler			eventHandler;
	MessageReceivedHandler	msgHandler;
};

#endif // !TCPCLIENT_H