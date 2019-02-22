#pragma once

#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <map>
#include "WebClient.h"
#include "Response.h"
#include "Global.h"

class Request
{
public:
	Request(WebClient sendingClient, std::string request) :
		sendingClient(sendingClient), m_request(request) { }

	std::string parse();									// parses request string

private:
	std::string sendBack(int code, std::string content);	// sends back content

	WebClient sendingClient;							// client that sent it
	std::string m_request;								// request body
	Method method;										// method of request (GET or POST)
	std::string route;									// request route
	std::string protocol;								// request protocol
	std::string host;									// request host
	std::map<std::string, std::string> data;			// request form data
};

#endif