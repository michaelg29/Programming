#include <iostream>

#include "json.h"
#include "util.h"

using namespace json;

int main() {
	std::cout << "Hello, JSON!" << std::endl;

	json_object d = json_util::read("data.json");
	//std::cout << d.getType() << ' ' << d.stringify() << std::endl;

	//json_util::write("data2.json", d);

	json_data test { {"hello", 5 } };

	d["json"] = { {"hello", 5 } };
	d["json"]["hello"] = 7;

	std::cout << d["json"]["hello"].stringify() << std::endl;

	std::cout << "Goodbye, JSON!" << std::endl;
}