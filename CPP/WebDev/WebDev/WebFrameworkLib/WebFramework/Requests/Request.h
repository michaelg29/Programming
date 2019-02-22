#pragma once

#ifndef REQUEST_H
#define REQUEST_H

#include "../WebFramework.h"

#include "../Sockets/WebServer.h"
#include "../Sockets/WebClient.h"
#include "../Sockets/WebServerAttributes.h"
#include "../Util/Util.h"

#include <string>
#include <map>

class Request
{
public:
	Request(WebClient sendingClient, std::string request, WebServerAttributes server_atts) :
		sendingClient(sendingClient), m_request(request), server_atts(server_atts) { }

	void parse();											// parses request string
	void forward();											// forward request
	void setContent(std::string content);					// set content
	void readFile(std::string filePath);					// link request to file path

	std::string							route;				// request route

private:
	WebClient							sendingClient;		// client that sent it
	WebServerAttributes					server_atts;		// attributes of hosting server
	std::string							m_request;			// request body
	std::string							method;				// method of request
	std::string							protocol;			// request protocol
	std::string							host;				// request host
	std::map<std::string, std::string>	data;				// request form data

	std::string							output_content;		// content to be sent back
	int									output_code;		// output code
};

#endif