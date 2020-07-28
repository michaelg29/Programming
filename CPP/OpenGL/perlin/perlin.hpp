#ifndef PERLIN_HPP
#define PERLIN_HPP

#include <iterator>
#include <algorithm>
#include <stdlib.h>
#include <random>

namespace perlin {
	/*
		utility methods
	*/

	// fade function: f(t) = 6t^5 - 15t^4 + 10t^3 (optimized for fewer mutliplications)
	inline float fade(float t) {
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	// linear interpolation: x(t) = x1 + t * (x2 - x1)
	inline float lerp(float t, float a, float b) {
		return a + t * (b - a);
	}

	// generate gradient vector for points
	// calculates dot product between gradient vector and position vector
	inline float grad(int hash, float x, float y, float z) {
		// convert last 4 bits of hash into one of 12 possible gradients
		int h = hash & 0b1111; // = hash % 16 = get last 4 bits

		// if first bit 1, set to x, otherwise set to y
		float u = h < 0b1000 ? x : y;

		// if first/second bits 0, set to y
		// if first/second bits 1, set to x
		// else set to z
		float v = h < 0b0100 ? y : h == 0b1100 || h == 0b1110 ? x : z;

		// make u, v negative based on last 2 bits, then add results
		return ((h & 0b0001) == 0 ? u : -u) + ((h & 0b0010) == 0 ? v : -v);
	}

	// map function
	inline float map(float val, float ogMin, float ogMax, float newMin, float newMax) {
		// get distance in original range
		float prop = (val - ogMin) / (ogMax - ogMin);

		return newMin + prop * (newMax - newMin);
	}

	/*
		main class
	*/

	class Perlin {
	private:
		// permutation array
		unsigned char p[512];

		// current seed
		unsigned int seed;

	public:
		/*
			constructor
		*/
		Perlin(int seed) {
			reseed(seed);
		}

		// method to reseed permutation table
		void reseed(unsigned int _seed) {
			seed = _seed;
			srand(seed);

			// populate array in order
			for (unsigned int i = 0; i < 256; i++) {
				p[i] = i;
			}

			// shuffle first half
			std::shuffle(std::begin(p), std::begin(p) + 256, std::default_random_engine(seed));

			// duplicate array for overflow
			for (unsigned int i = 0; i < 256; i++) {
				p[256 + i] = p[i];
			}
		}

		/*
			perlin methods
		*/

		// 1D Perlin noise
		float noise1D(float x) {
			// find interval containing point
			int xi = (int)(std::floorf(x)) & 255;

			// get decimal value of each component
			x -= std::floorf(x);

			// get smooth value from fade function (becomes weight for each dimension)
			float u = fade(x);

			// hash function for all 4 unit square coordinates
			int a, b;
			a = p[xi    ];
			b = p[xi + 1];

			// get weighted average
			float avg = lerp(
				u,
				grad(a, x, 0, 0),
				grad(b, x - 1, 0, 0)
			);

			// return avg mapped from [-1, 1] to [0, 1]
			return map(avg, -1, 1, 0, 1);
		}

		// 2D Perlin noise
		float noise2D(float x, float y) {
			// find square containing point
			int xi = (int)(std::floorf(x)) & 255;
			int yi = (int)(std::floorf(y)) & 255;

			// get decimal value of each component
			x -= std::floorf(x);
			y -= std::floorf(y);

			// get smooth value from fade function (becomes weight for each dimension)
			float sx = fade(x);
			float sy = fade(y);

			// hash function for all 4 unit square coordinates
			int aa, ab, ba, bb;
			aa = p[p[xi    ] + yi    ];
			ab = p[p[xi    ] + yi + 1];
			ba = p[p[xi + 1] + yi    ];
			bb = p[p[xi + 1] + yi + 1];

			// get weighted average
			float avg = lerp(
				sy,
				lerp(
					sx, // "top"
					grad(aa, x    , y    , 0),
					grad(ba, x - 1, y    , 0)
				),
				lerp(
					sx, // "bottom"
					grad(ab, x    , y - 1, 0),
					grad(bb, x - 1, y - 1, 0)
				)
			);

			// return avg mapped from [-1, 1] to [0, 1]
			return map(avg, -1, 1, 0, 1);
		}

		// 3D Perlin noise
		float noise3D(float x, float y, float z) {
			// find cube containing point
			int xi = (int)(std::floorf(x)) & 255;
			int yi = (int)(std::floorf(y)) & 255;
			int zi = (int)(std::floorf(z)) & 255;

			// get decimal value of each component
			x -= std::floorf(x);
			y -= std::floorf(y);
			z -= std::floorf(z);

			// get smooth value from fade function (becomes weight for each dimension)
			float u = fade(x);
			float v = fade(y);
			float w = fade(z);

			// hash function for all 8 unit cube coordinates
			unsigned char aaa, aba, aab, abb, baa, bba, bab, bbb;
			aaa = p[p[p[xi    ] + yi    ] + zi    ];
			aba = p[p[p[xi    ] + yi + 1] + zi    ];
			aab = p[p[p[xi    ] + yi    ] + zi + 1];
			abb = p[p[p[xi    ] + yi + 1] + zi + 1];
			baa = p[p[p[xi + 1] + yi    ] + zi    ];
			bba = p[p[p[xi + 1] + yi + 1] + zi    ];
			bab = p[p[p[xi + 1] + yi    ] + zi + 1];
			bbb = p[p[p[xi + 1] + yi + 1] + zi + 1];

			// get weighted average
			float avg = lerp(
				w,
				lerp(
					v, // "front"
					lerp(
						u, // "top"
						grad(aaa, x    , y    , z    ),
						grad(baa, x - 1, y    , z    )
					),
					lerp(
						u, // "bottom"
						grad(aba, x    , y - 1, z    ),
						grad(bba, x - 1, y - 1, z    )
					)
				),
				lerp(
					// "rear"
					v,
					lerp(
						u, // "top"
						grad(aab, x    , y    , z - 1),
						grad(bab, x - 1, y    , z - 1)
					),
					lerp(
						u, // "bottom"
						grad(abb, x    , y - 1, z - 1),
						grad(bbb, x - 1, y - 1, z - 1)
					)
				)
			);

			// return avg mapped from [-1, 1] to [0, 1]
			return map(avg, -1, 1, 0, 1);
		}

		// 2d accumulated noise
		float accumulatedOctaveNoise2D(float x, float y, int octaves = 8, float lacunarity = 2.0f, float gain = 0.5f) {
			float result = 0.0f;
			float amplitude = 1.0f;
			float frequency = 1.0f;
			float maxVal = 0.0f; // used to normalize result

			for (; octaves > 0; octaves--) {
				result += noise2D(x * frequency, y * frequency) * amplitude;
				
				maxVal += amplitude;

				amplitude *= gain;
				frequency *= lacunarity;
			}

			return result / maxVal;
		}
	};
}

#endif