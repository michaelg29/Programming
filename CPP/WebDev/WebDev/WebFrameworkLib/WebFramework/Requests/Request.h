#pragma once

#ifndef REQUEST_H
#define REQUEST_H

#include "../WebFramework.h"

#include "../Sockets/WebClient.h"
#include "../Util/Util.h"

#include <string>
#include <map>

class Request
{
public:
	Request(WebClient sendingClient, std::string request, WebServerAttributes server_atts) :
		sendingClient(sendingClient), m_request(request) { }

	std::string parse();									// parses request string

private:
	std::string sendBack(int code, std::string content);	// sends back content

	WebClient							sendingClient;		// client that sent it
	WebServerAttributes					server_atts;		// attributes of hosting server
	std::string							m_request;			// request body
	std::string							method;				// method of request
	std::string							route;				// request route
	std::string							protocol;			// request protocol
	std::string							host;				// request host
	std::map<std::string, std::string>	data;				// request form data
};

#endif