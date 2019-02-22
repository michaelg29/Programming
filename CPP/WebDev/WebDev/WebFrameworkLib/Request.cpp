#include "WebFramework/Requests/Request.h"

#include <istream>
#include <sstream>
#include <fstream>
#include <vector>
#include <iterator>

#include <iostream>

std::string Request::parse() {
	std::istringstream iss(m_request);
	std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());

	method = parsed[0];
	route = server_atts.getFilePath(parsed[1]);
	std::cout << parsed[1] << " --- " << route << std::endl;
	protocol = parsed[3];
	host = parsed[7];

	std::string params = parsed[parsed.size() - 1];

	if (params.find('=') < params.length() && params.find('=') > -1) {
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

	int code = 404;
	std::string content = "";

	// Read file
	// Open the document in the local file system
	std::ifstream f(route);

	// Check if it opened and if it did, grab the entire contents
	if (f.good())
	{
		std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());

		for (std::pair<std::string, std::string> pair : sendingClient.context) {
			WebServerUtil::replace_all(str, "{{ " + pair.first + " }}", pair.second);
		}

		content = str;
		code = 200;

		if (route == server_atts.contextRoute + "/" + server_atts.errorFile)
			code = 404;
	}

	f.close();

	return sendBack(code, content);
}

std::string Request::sendBack(int code, std::string content) {
	// write the document back to the client
	std::ostringstream oss;
	oss << "HTTP/1.1 " << code << " OK\r\n";
	oss << "Cache-Control: no-cache, private\r\n";
	oss << "Content-Type: text/html\r\n";
	oss << "Content-Length: " << content.size() << "\r\n";
	oss << "\r\n";
	oss << content;

	std::string output = oss.str();

	return output;
}