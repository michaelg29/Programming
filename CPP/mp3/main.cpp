#include <iostream>

#include "mp3.hpp"

int main() {
	std::cout << "Hello, world!" << std::endl;

	ID3::load();

	MP3 f;
	f.read("C418 - Key.mp3");
	
	f.eraseTag("APIC");
	f.setData("TRCK", 9, {
		'1',
		0x00,
		'/',
		0x00,
		'3',
		0x00,
		'4',
		0x00,
		'5',
		0x00,
		'6',
		0x00
	});

	f.write("newC418 - Key.mp3");

	ID3::clear();

	return 0;
}