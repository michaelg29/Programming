#include <TcpListener.h>
#include <iostream>
#include <thread>
#include <string>

void MsgHandler(TcpListener* listener, int socketId, std::string msg);
void EventHandler(TcpListener* listener, std::string msg);
void CmdHandler(TcpListener* listener, int socket, std::string cmd);
void CommandListener(TcpListener *listener);

int main() {
	TcpListener l("192.168.1.193", 54000, "\\", MsgHandler, EventHandler, CmdHandler);

	if (l.init() == 0) {
		std::thread cmdThread(CommandListener, &l);
		l.run();
		
		cmdThread.join();
	}
	else {

	}

	system("pause");
}

void CommandListener(TcpListener *listener) {
	std::string cmd;
	while (true) {
		getline(std::cin, cmd);

		if (cmd == "\\exit" || cmd == "\\quit") {
			listener->disconnect();

			break;
		}
	}
}

void MsgHandler(TcpListener* listener, int client, std::string msg) {
	/*ServerMessage m = ServerMessage::Parse(msg);

	std::cout << "Client " << client << " sends a " << m.type << " message: ";

	for (auto pair : m.attributes) {
		std::cout << "\n\t" << pair.first << " = " << pair.second;
	}*/

	std::cout << "\n\t" << client << " says " << msg << std::endl;
}

void EventHandler(TcpListener* listener, std::string msg) {
	std::cout << "Server says: " << msg << std::endl;
}

void CmdHandler(TcpListener* listener, int socket, std::string cmd) {
	// Is the command quit? 
	if (cmd == "\\quit")
	{
		std::string disconnectMsg = "Goodbye. Sorry to see you go!\r\n";
		listener->disconnectClient(socket);
	}

	// Does the command set the name
	if (cmd.find("\\name") < cmd.length()) {
		for (NetworkNode &node : listener->clients) {
			if (socket == node.socket) {
				node.attributes["displayName"] = cmd.substr(cmd.find(' ') + 1, cmd.find_last_of('\n'));

				std::string response = "Your name is now " + std::string(node.attributes["displayName"]);
				send(socket, response.c_str(), response.size() + 1, 0);
			}
		}
	}
}