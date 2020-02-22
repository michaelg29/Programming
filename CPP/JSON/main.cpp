#include <iostream>

#include "json.h"
#include "util.h"

using namespace json;

int main() {
	std::cout << "Hello, JSON!" << std::endl;

	json_data d = json_util::read("data.json");
	std::cout << d.getType() << ' ' << d.stringify() << std::endl;

	//std::string data = "{\"hello\": 3, \"name\": true}";
	//json_data jdata = json_data::parse(data);
	//std::cout << jdata.getType() << ' ' << jdata.stringify() << std::endl;
}