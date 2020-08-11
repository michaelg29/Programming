#include <iostream>

#include "mp3.hpp"

int main() {
	std::cout << "Hello, world!" << std::endl;

	ID3::load();

	MP3 f;
	f.read("C418 - Beginning.mp3");
	
	f.write("newC418 - Beginning.mp3");

	ID3::clear();

	return 0;
}