#include <iostream>

#include "mp3.hpp"

int main() {
	std::cout << "Hello, world!" << std::endl;

	ID3::load();

	MP3 f;
	f.read("C418 - Key.mp3");
	
	//f.id3v2Data["TRCK"][9] = '2';

	f.write("newC418 - Key.mp3");

	ID3::clear();

	return 0;
}