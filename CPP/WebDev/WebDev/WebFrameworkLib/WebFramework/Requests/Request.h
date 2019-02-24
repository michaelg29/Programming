#ifndef REQUEST
#define REQUEST

#include "../WebFramework.h"

#include "../Sockets/WebServer.h"
#include "../Sockets/WebClient.h"
#include "../Sockets/WebServerAttributes.h"
#include "../Util/Util.h"

#include <string>
#include <map>

class WEBFRAMEWORK_API Request
{
public:
	Request(WebClient sendingClient, std::string request, WebServerAttributes server_atts) :
		sendingClient(sendingClient), m_request(request), server_atts(server_atts) { }

	void parse();											// parses request string
	void forward();											// forward request
	void setContent(std::string content);					// set content
	void readFile(std::string filePath);					// link request to file path
	void setContext(std::string key, std::string value);	// sets value in client context
	std::map<std::string, std::string> context();			// returns client context

	WebClient							sendingClient;		// client that sent it
	WebServerAttributes					server_atts;		// attributes of hosting server
	std::string							m_request;			// request body

	std::string							output_content;		// content to be sent back
	int									output_code;		// output code
	std::string							route;				// request route
	std::string							method;				// method of request
	std::string							protocol;			// request protocol
	std::string							host;				// request host
	std::map<std::string, std::string>	data;				// request form data
};

#endif