#include <TcpClient.h>
#include <iostream>
#include <string>
#include <thread>

void _EventHandler(TcpClient* client, std::string msg);
void _MessageHandler(TcpClient* client, std::string msg);
void _InputListener(TcpClient* client);

int main() {
	TcpClient c("192.168.1.193", 54000, _EventHandler, _MessageHandler);

	if (c.Init() == 0) {
		std::thread inputThread(_InputListener, &c);

		c.Run();
		inputThread.join();
	}
	else {

	}

	system("pause");
	return 0;
}

void _EventHandler(TcpClient* client, std::string msg) {
	std::cout << "Client> " << msg << std::endl;
}

void _MessageHandler(TcpClient* client, std::string msg) {
	std::cout << "Server> " << msg << std::endl;
}

void _InputListener(TcpClient* client) {
	std::string input;

	while (true) {
		getline(std::cin, input);

		if (input.size() > 0) {
			if (input == "\\quit" || input == "\\exit" || input == "\\disconnect") {
				client->running = false;
				client->ForceDisconnect();
				client->Cleanup();
				break;
			}

			client->Send(input);
		}
	}
}