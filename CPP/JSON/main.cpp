#include <iostream>

#include "json.hpp"

int main() {
	std::cout << "Hello, JSON!" << std::endl;

    jsoncpp::json d = {
		{"pi", 3.141},
		{"happy", true},
		{"name", "Niels"},
		{"answer", {
			{"everything", 42}
		}},
		{"list", {1, 0, 2}},
		{"object", {
			{"currency", "USD"},
			{"value", 42.99}
		}}
    };

	std::cout << d.dump(4) << std::endl;
	d["list"] += 5;
	std::cout << d.dump(4) << std::endl;

	//std::cout << json::parse(d.dump()).dump(4) << std::endl;

	std::cout << "Goodbye, JSON!" << std::endl;
}