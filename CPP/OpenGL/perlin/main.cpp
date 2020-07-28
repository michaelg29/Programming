#include "image.hpp"
#include "perlin.hpp"

# include <iostream>

int main() {
	std::cout << "Hello, Ken!" << std::endl;

	perlin::Perlin map(46561698614);

	Image image;
	image.generate(512, 512);

	double frequency = 6;
	double fx = image.getWidth() / frequency;
	double fy = image.getHeight() / frequency;

	for (int y = 0; y < image.getHeight(); y++) {
		for (int x = 0; x < image.getWidth(); x++) {
			if (x == 132 && y == 241) {
				std::cout << "";
			}
			double p = map.accumulatedOctaveNoise2D(x / fx, y / fy);
			image.setPixel(x, y, p, p, p);
		}
	}

	image.write("out.bmp");

	return 0;
}