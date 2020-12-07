#include <iostream>
#include <limits>

#include "json2.hpp"

int main() {
	std::cout << "Hello, JSON!" << std::endl;

 //   jsoncpp2::json d = {
	//	{"pi", 3.141},
	//	{"happy", true},
	//	{"name", "Niels"},
	//	{"answer", {
	//		{"everything", 42}
	//	}},
	//	{"list", {1, 0, 2}},
	//	{"object", {
	//		{"currency", "USD"},
	//		{"value", 42.99}
	//	}},
	//	{"tes2", jsoncpp2::null}
 //   };
	//d["test"] = jsoncpp2::null;

	////jsoncpp2::json d = jsoncpp2::read("test.json");

	//std::cout << d.dump(4) << std::endl;

	//d.dump("test.json", 4);

	//std::cout << jsoncpp2::parse(d.dump()).dump(4) << std::endl;

	jsoncpp2::json d = jsoncpp2::read("data.json");
	d.dump("spacex.json", 4);

	std::cout << "Goodbye, JSON!" << std::endl;
}