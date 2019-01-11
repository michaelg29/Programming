#include "TcpListener.h"

TcpListener::TcpListener(std::string ipAddress,
	int port,
	std::string cmdIndicator,
	MessageReceivedHandler msgHandler,
	ServerEventHandler eventHandler,
	CommandHandler cmdHandler) 
	: m_ipAddress(ipAddress),
	m_port(port),
	cmdIndicator(cmdIndicator),
	msgHandler(msgHandler),
	eventHandler(eventHandler),
	cmdHandler(cmdHandler)
{}

TcpListener::~TcpListener() {}

int TcpListener::init() {
	WSADATA data;
	WORD ver = MAKEWORD(2, 2);

	return WSAStartup(ver, &data);
}

void TcpListener::run() {
	// holds information buffer
	char buf[MAX_BUFFER_SIZE];

	// clears out client list
	FD_ZERO(&master);

	// create socket
	m_node = createSocket();
	if (m_node.socket == INVALID_SOCKET) {
		eventHandler(this, "Could not start server");
		return;
	}

	// start server
	FD_SET(m_node.socket, &master);
	eventHandler(this, "Server started");
	m_running = true;

	// main processing loop
	while (m_running) {
		// create copy of list
		fd_set copy = master;
		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

		for (int i = 0; i < socketCount; i++) {
			SOCKET sock = copy.fd_array[i];

			if (sock == m_node.socket) {
				// accept new connection
				NetworkNode clientNode = getConnection();

				std::ostringstream connect_msg;
				connect_msg << clientNode.getName() << " connected at port " << clientNode.getPort();
				eventHandler(this, connect_msg.str());

				clients.push_back(clientNode);
				FD_SET(clientNode.socket, &master);
			}
			else {
				// receive inbound message
				int bytesIn = recv(sock, buf, MAX_BUFFER_SIZE, 0);

				if (bytesIn <= 0) {
					// disconnection
					disconnectClient(sock);
					continue;
				}
				else if (buf != "" && buf != "\n") {
					// ensure message comes through properly and uncorrupted
					std::string incoming = std::string(buf, bytesIn) + '\0';
					strcpy_s(buf, incoming.c_str());

					// check for command
					if (incoming.find(cmdIndicator) == 0) {
						cmdHandler(this, sock, incoming);
						continue;
					}

					msgHandler(this, sock, incoming);
				}
			}
		}
	}

	disconnect();
}

NetworkNode TcpListener::createSocket() {
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
		goto Error;
	else {
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(m_port);
		inet_pton(AF_INET, m_ipAddress.c_str(), &hint.sin_addr);

		int bindInfo = bind(listening, (sockaddr*)&hint, sizeof(hint)); // binds socket to ip address and port

		if (bindInfo == SOCKET_ERROR)
			goto Error;
		else {
			int listenInfo = listen(listening, SOMAXCONN); // tells compiler that socket is for listening

			if (listenInfo == SOCKET_ERROR)
				goto Error;
		}

		NetworkNode node;
		node.hint = hint;
		node.socket = listening;

		return node;
	}

Error:
	NetworkNode errorNode;
	errorNode.socket = INVALID_SOCKET;
	return errorNode;
}

// Get connection
NetworkNode TcpListener::getConnection() {
	sockaddr_in clientInfo;
	int clientSize = sizeof(clientInfo);

	SOCKET client = accept(m_node.socket, (sockaddr*)&clientInfo, &clientSize);

	NetworkNode node;
	node.socket = client;
	node.hint = clientInfo;

	return node;
}

// Drop Client
void TcpListener::disconnectClient(SOCKET client) {
	closesocket(client);
	FD_CLR(client, &master);

	for (NetworkNode node : clients) {
		if (client == node.socket) {
			std::ostringstream disconnect_msg;
			disconnect_msg << node.getName() << " disconnected at port " << node.getPort();
			eventHandler(this, disconnect_msg.str());
			break;
		}
	}
}

// Force disconnect
void TcpListener::disconnect() {
	std::string msg = "Server is shutting down. Goodbye\n";

	eventHandler(this, msg);

	while (master.fd_count > 0) {
		SOCKET sock = master.fd_array[0];

		// remove socket from master list and close it
		FD_CLR(sock, &master);
		closesocket(sock);
	}

	FD_CLR(m_node.socket, &master);
	closesocket(m_node.socket);

	clients.clear();
}

// Cleanup
void TcpListener::cleanup() {
	WSACleanup();
}

// Send a message to a client, if client == -1, sends to all
void TcpListener::send_msg(std::string msg, SOCKET client) {
	if (client == -1) {
		// send to all clients
		for (SOCKET sock : master.fd_array) {
			send(sock, msg.c_str(), msg.size() + 1, 0);
		}
	}
	else {
		send(client, msg.c_str(), msg.size() + 1, 0);
	}
}