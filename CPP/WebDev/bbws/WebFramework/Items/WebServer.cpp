#include "WebServer.h"
#include "Request.h"
#include <iostream>

void WebServer::onClientConnected(int clientSocket) {
	std::map<std::string, std::string> context;
	clients.push_back(WebClient(clientSocket));
}

void WebServer::onClientDisconnected(int clientSocket) {

}

void WebServer::onMessageReceived(int clientSocket, const char* msg, int length) {
	for (WebClient client : clients) {
		if (client.socket == clientSocket) {
			Request request = Request(client, msg);
			std::string response = request.parse();

			int size = response.size() + 1;

			sendToClient(clientSocket, response.c_str(), size);
		}
	}

	//// parse document requested
	//std::istringstream iss(msg);
	//std::vector<std::string> parsed((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
	//

	//// Some defaults for output to the client (404 file not found 'page')
	//std::string content = "<h1>404 Not Found</h1>";
	//std::string htmlFile = "/index.html";
	//int errorCode = 404;

	//// If the GET request is valid, try and get the name
	//if (parsed.size() >= 3 && (parsed[0] == "GET" || parsed[0] == "POST"))
	//{
	//	htmlFile = parsed[1];

	//	if (parsed[0] == "POST") {
	//		std::string params = parsed[parsed.size() - 1];
	//		std::string key = "", val = "";
	//		bool addingToKey = true;
	//		for (std::string::size_type i = 0; i < params.size(); ++i) {
	//			char c = params[i];
	//			std::cout << c << std::endl;
	//			if (c == '=') {
	//				addingToKey = false;
	//				continue;
	//			}
	//			else if (c == '&' || i == params.size() - 1) {
	//				addingToKey = true;
	//				context[key] = val + c;
	//				key = "";
	//				val = "";
	//				std::cout << key << ' ' << val << std::endl;
	//				continue;
	//			}
	//			
	//			if (addingToKey)
	//				key += c;
	//			else
	//				val += c;
	//		}
	//	}

	//	// If the file is just a slash, use index.html. This should really
	//	// be if it _ends_ in a slash. I'll leave that for you :)
	//	if (htmlFile == "/")
	//	{
	//		htmlFile = "/" + defaultFile;
	//	}
	//}

	//// Open the document in the local file system
	//std::ifstream f(".\\wwwroot" + htmlFile);

	//// Check if it opened and if it did, grab the entire contents
	//if (f.good())
	//{
	//	std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());

	//	for (std::pair<std::string, std::string> pair : context) {
	//		replace_all(str, "{{ " + pair.first + " }}", pair.second);
	//	}

	//	content = str;
	//	errorCode = 200;
	//}

	//f.close();

	//// write the document back to the client
	//std::ostringstream oss;
	//oss << "HTTP/1.1 " << errorCode << " OK\r\n";
	//oss << "Cache-Control: no-cache, private\r\n";
	//oss << "Content-Type: text/html\r\n";
	//oss << "Content-Length: " << content.size() << "\r\n";
	//oss << "\r\n";
	//oss << content;

	//std::string output = oss.str();
	//int size = output.size() + 1;

	
}
