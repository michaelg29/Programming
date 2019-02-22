#include "WebServer.h"
#include <iostream>
#include <string>

void main()
{
	WebServer ws("127.0.0.1", 8080);
	if (ws.init() != 0)
		return;

	ws.run();

	system("pause");
	return;
}
