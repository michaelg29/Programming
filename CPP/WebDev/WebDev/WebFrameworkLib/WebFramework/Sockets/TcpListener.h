#ifndef TCPLISTENER
#define TCPLISTENER

#include "../WebFramework.h"

#include <WS2tcpip.h>
#include <vector>

#pragma comment (lib, "ws2_32.lib")

class WEBFRAMEWORK_API TcpListener {
public:

	// initialize variables
	TcpListener(const char* ipAddress, int port) :
		m_ipAddress(ipAddress), m_port(port) { }

	// initialize listener
	int init();

	// run listener
	int run();

protected:

	// handler for client connections
	virtual void onClientConnected(int clientSocket);

	// handler for client disconnections
	virtual void onClientDisconnected(int clientSocket);

	// handler for message from client
	virtual void onMessageReceived(int clientSocket, const char* msg, int length);

	// send message to client
	void sendToClient(int clientSocket, const char* msg, int length);

	// broadcast message from client
	void broadcast(int sendingClient, const char* msg, int length);

private:

	const char*				m_ipAddress;	// ip address server will run on
	int						m_port;			// port number for web service
	int						m_socket;		// internal FD for the listening socket
	fd_set					m_master;		// master file descriptor set
};

#endif