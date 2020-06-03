#include <iostream>
#include <limits>

#include "json.hpp"

int main() {
	std::cout << "Hello, JSON!" << std::endl;

    /*jsoncpp::json d = {
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

	d.dump("data.json", 4);*/

	//std::cout << json::parse(d.dump()).dump(4) << std::endl;

	jsoncpp::json d = jsoncpp::read("data.json");
	d.dump("spacex.json", 4);

	std::cout << "Goodbye, JSON!" << std::endl;
}