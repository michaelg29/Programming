/**
	NetworkNode.h
	Purpose: Hold information about each node on the network

	@author Michael Grieco
	@created 01/10/2019
*/

#include <map>
#include <WS2tcpip.h>
#include <string>

// struct containing network information
struct NetworkNode {
	SOCKET socket;									// socket object
	sockaddr_in hint;								// connection information
	std::map<std::string, std::string> attributes;	// Ma with node attributes (customly defined)

	char* getName();								// retrieves connection name of node
	int getPort();									// retrieves port the node is listening on
};