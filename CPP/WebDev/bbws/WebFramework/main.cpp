#include "Items/WebServer.h"

void main() {
	WebServer ws("192.168.1.157", 8080);

	if (ws.init() != 0)
		return;

	ws.run();

	system("pause");
}