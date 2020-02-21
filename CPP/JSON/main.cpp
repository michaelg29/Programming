#include <iostream>

#include "json.h"
#include "util.h"

using namespace json;

int main() {
	std::cout << "Hello, JSON!" << std::endl;

	std::string data = "true";
	json_data jdata = json_data::parse(data);


	std::cout << jdata.getType() << ' ' << jdata.stringify() << std::endl;
}