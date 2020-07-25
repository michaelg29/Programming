#include "consoletools.hpp"
#include <time.h>
#include <iostream>

int main() {
	/*std::cout << "====================" << std::endl
		<< "CONSOLE TOOLS" << std::endl
		<< "By Michael Grieco" << std::endl
		<< "====================" << std::endl
		<< "Type 'stop' to finish" << std::endl << std::endl;

	std::cout << "Thank you for using Console Tools" << std::endl;

	time_t result = time(NULL);

	char str[26];
	ctime_s(str, sizeof(str), &result);
	printf("%s", str);*/

	App app("Console Tools", "Michael Grieco");
	app.init();

	while (true) {
		std::string in;
		std::cin >> in;
		if (in == "stop") {
			break;
		}
	}

	app.cleanup();

	system("pause");

	return 0;
}