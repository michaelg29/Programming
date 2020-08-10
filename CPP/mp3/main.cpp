#include <iostream>

#include "mp3.hpp"

struct Letters {
	static char abcd[4];
	static char efgh[4];
	static char ijkl[4];
};

char Letters::abcd[4] = { 0 };
char Letters::efgh[4] = { 0 };
char Letters::ijkl[4] = { 0 };

int main() {
	std::cout << "Hello, world!" << std::endl;

	ID3::load();

	MP3 f;
	f.read("C418 - Beginning.mp3");
	
	f.id3v1.title[0] = 'C';
	f.id3v1.artist[1] = '5';

	f.write("newC418 - Beginning2.mp3");

	ID3::clear();

	return 0;
}