#include "WebFramework/Requests/Request.h"

#include <istream>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>

void Request::parse() {
	std::istringstream iss(m_request);
	std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

	method = parsed[0];
	route = parsed[1];
	protocol = parsed[3];
	host = parsed[7];

	std::string params = parsed[parsed.size() - 1];
	
	if (method == "POST") {
		std::string key = "", val = "";
		bool addingToKey = true;
		for (std::string::size_type i = 0; i < params.size(); ++i) {
			char c = params[i];
			if (c == '=') {
				addingToKey = false;
				continue;
			}
			else if (c == '&' || i == params.size() - 1) {
				addingToKey = true;
				data[key] = val + c;
				key = "";
				val = "";
				continue;
			}

			if (addingToKey)
				key += c;
			else
				val += c;
		}
	}
}

void Request::setContent(std::string content) {
	output_content = content;
	output_code = 200;
}

void Request::readFile(std::string filePath) {
	output_content = WebServerUtil::FileParser::parseFile(filePath, this);
}

void Request::forward() {
	// write the document back to the client
	std::ostringstream oss;
	oss << "HTTP/1.1 " << output_code << " OK\r\n";
	oss << "Cache-Control: no-cache, private\r\n";
	oss << "Content-Type: text/html\r\n";
	oss << "Content-Length: " << output_content.size() << "\r\n";
	oss << "\r\n";
	oss << output_content;

	std::string output = oss.str();
	int size = output.size() + 1;

	send(sendingClient.socket, output.c_str(), size, 0);
}

void Request::setContext(std::string key, std::string value) {
	sendingClient.context[key] = value;
}

std::map<std::string, std::string> Request::context() {
	return sendingClient.context;
}