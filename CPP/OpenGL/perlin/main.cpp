#include "bmp.h"
#include "image.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

int main() {
	const char* infile = "in.bmp";
	const char* outfile = "out.bmp";

	Image img;
	img.read(infile);
	img.rotate180();
	img.write(outfile);
	img.clear();

	return 0;
}