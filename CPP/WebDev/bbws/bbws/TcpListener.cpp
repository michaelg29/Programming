#include "TcpListener.h"

#include <iostream>
#include <string>
#include <sstream>

int TcpListener::init() {
	// Initialze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		return wsOk;
	}

	// Create a socket
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET)
	{
		return WSAGetLastError();
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(m_port);
	inet_pton(AF_INET, m_ipAddress, &hint.sin_addr);

	if (bind(m_socket, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR)
		return WSAGetLastError();

	// Tell Winsock the socket is for listening 
	if (listen(m_socket, SOMAXCONN) == SOCKET_ERROR)
		return WSAGetLastError();
	
	// Create the master file descriptor set and zero it
	FD_ZERO(&m_master);

	// Add our first socket that we're interested in interacting with; the listening socket!
	// It's important that this socket is added for our server or else we won't 'hear' incoming
	// connections 
	FD_SET(m_socket, &m_master);

	return 0;
}

int TcpListener::run() {
	// this will be changed by the \quit command (see below, bonus not in video!)
	bool running = true;

	while (running)
	{
		// Make a copy of the master file descriptor set, this is SUPER important because
		// the call to select() is _DESTRUCTIVE_. The copy only contains the sockets that
		// are accepting inbound connection requests OR messages. 

		// E.g. You have a server and it's master file descriptor set contains 5 items;
		// the listening socket and four clients. When you pass this set into select(), 
		// only the sockets that are interacting with the server are returned. Let's say
		// only one client is sending a message at that time. The contents of 'copy' will
		// be one socket. You will have LOST all the other sockets.

		// SO MAKE A COPY OF THE MASTER LIST TO PASS INTO select() !!!

		fd_set copy = m_master;

		// See who's talking to us
		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

		// Loop through all the current connections / potential connect
		for (int i = 0; i < socketCount; i++)
		{
			// Makes things easy for us doing this assignment
			SOCKET sock = copy.fd_array[i];

			// Is it an inbound communication?
			if (sock == m_socket)
			{
				// Accept a new connection
				SOCKET client = accept(m_socket, nullptr, nullptr);

				// Add the new connection to the list of connected clients
				FD_SET(client, &m_master);

				onClientConnected(client);
			}
			else // It's an inbound message
			{
				char buf[4096];
				ZeroMemory(buf, 4096);

				// Receive message
				int bytesIn = recv(sock, buf, 4096, 0);
				if (bytesIn <= 0)
				{
					// Drop the client
					closesocket(sock);
					FD_CLR(sock, &m_master);
					onClientDisconnected(sock);
				}
				else
				{
					onMessageReceived(sock, buf, bytesIn);
					//// Check to see if it's a command. \quit kills the server
					//if (buf[0] == '\\')
					//{
					//	// Is the command quit? 
					//	std::string cmd = std::string(buf, bytesIn);
					//	if (cmd == "\\quit")
					//	{
					//		running = false;
					//		break;
					//	}

					//	// Unknown command
					//	continue;
					//}

					// Send message to other clients, and definiately NOT the listening socket

					//for (int i = 0; i < m_master.fd_count; i++)
					//{
					//	SOCKET outSock = m_master.fd_array[i];
					//	if (outSock != m_socket && outSock != sock)
					//	{
					//		/*std::ostringstream ss;
					//		ss << "SOCKET #" << sock << ": " << buf << "\r\n";
					//		std::string strOut = ss.str();

					//		send(outSock, strOut.c_str(), strOut.size() + 1, 0);*/
					//	}
					//}
				}
			}
		}
	}

	// Remove the listening socket from the master file descriptor set and close it
	// to prevent anyone else trying to connect.
	FD_CLR(m_socket, &m_master);
	closesocket(m_socket);

	//// Message to let users know what's happening.
	//std::string msg = "Server is shutting down. Goodbye\r\n";

	while (m_master.fd_count > 0)
	{
		// Get the socket number
		SOCKET sock = m_master.fd_array[0];

		//// Send the goodbye message
		//send(sock, msg.c_str(), msg.size() + 1, 0);

		// Remove it from the master file list and close the socket
		FD_CLR(sock, &m_master);
		closesocket(sock);
	}

	// Cleanup winsock
	WSACleanup();

	return 0;
}

void TcpListener::sendToClient(int clientSocket, const char* msg, int length) {
	send(clientSocket, msg, length, 0);
}

void TcpListener::broadcast(int sendingClient, const char* msg, int length) {
	for (int i = 0; i < m_master.fd_count; i++)
	{
		SOCKET outSock = m_master.fd_array[i];
		if (outSock != m_socket && outSock != sendingClient)
		{
			sendToClient(outSock, msg, length);
		}
	}
}

void TcpListener::onClientConnected(int clientSocket) {

}

void TcpListener::onClientDisconnected(int clientSocket) {

}

void TcpListener::onMessageReceived(int clientSocket, const char* msg, int length) {

}