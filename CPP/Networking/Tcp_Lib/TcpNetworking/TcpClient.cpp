#include "TcpNetworking/TcpClient.h"
#include <sstream>

TcpClient::TcpClient(std::string ipAddress, int port, EventHandler eventHandler, MessageReceivedHandler msgHandler)
	:m_ipAddress(ipAddress), m_port(port), eventHandler(eventHandler), msgHandler(msgHandler)
{

}

TcpClient::~TcpClient() {

}

// Initialize winsock
int TcpClient::Init() {
	WSAData data;
	WORD ver = MAKEWORD(2, 2);

	return WSAStartup(ver, &data);
}

// Receive communications loop - main processing
void TcpClient::Run() {
	if (!CreateSocket()) {
		eventHandler(this, "Could not create socket");
		ForceDisconnect();
		Cleanup();
	}

	if (Connect() == SOCKET_ERROR) {
		eventHandler(this, "Could not connect to the server");
	}

	running = true;

	char buf[4096];

	while (running) {
		// get message
		ZeroMemory(buf, 4096);
		int bytesReceived = recv(m_socket, buf, 4096, 0);
		if (bytesReceived > 0) {
			msgHandler(this, std::string(buf, 0, bytesReceived));
		}
		else {
			running = false;
		}
	}
	eventHandler(this, "Disconnected");
	ForceDisconnect();
	Cleanup();
}

// Create the client socket
bool TcpClient::CreateSocket() {
	// Create socket
	m_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socket == INVALID_SOCKET) {
		Cleanup();
		return false;
	}

	// Fill in the hint structure
	m_hint.sin_family = AF_INET;
	m_hint.sin_port = htons(m_port);
	inet_pton(AF_INET, m_ipAddress.c_str(), &m_hint.sin_addr);

	return true;
}

// Connect to the socket at the ip address and port
int TcpClient::Connect() {
	return connect(m_socket, (sockaddr*)&m_hint, sizeof(m_hint));
}

// Force disconnect the server
void TcpClient::ForceDisconnect() {
	closesocket(m_socket);
}

// Cleanup
void TcpClient::Cleanup() {
	WSACleanup();
}

// Send a message to a server
void TcpClient::Send(std::string msg) {
	send(m_socket, msg.c_str(), msg.size() + 1, 0);
}
