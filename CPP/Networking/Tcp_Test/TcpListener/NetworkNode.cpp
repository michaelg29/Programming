#include "NetworkNode.h"

char* NetworkNode::getName() {
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	int result = getnameinfo((sockaddr*)&hint, sizeof(hint), host, NI_MAXHOST, service, NI_MAXSERV, 0);
	if (result != 0)
		inet_ntop(AF_INET, &hint.sin_addr, host, NI_MAXHOST);

	return host;
}

int NetworkNode::getPort() {
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	int result = getnameinfo((sockaddr*)&hint, sizeof(hint), host, NI_MAXHOST, service, NI_MAXSERV, 0);

	if (result == 0)
		return std::stoi(service);
	
	inet_ntop(AF_INET, &hint.sin_addr, host, NI_MAXHOST);
	return (int)ntohs(hint.sin_port);
}