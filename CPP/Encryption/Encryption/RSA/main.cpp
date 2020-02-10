#include <cmath>
#include <iostream>

#include <BigIntegerLibrary.hh>
#include <BigInteger.hh>

// (x ^ y) % p
BigInteger pow(BigInteger x, BigInteger y, BigInteger p) {
	BigInteger ret = 1;

	x %= p;

	while (y > 0) {
		// y odd
		if (y % 2 == 1) {
			ret = (ret * x) % p;
		}

		// y now even
		y /= 2;
		x = (x * x) % p;
	}

	return ret;
}

// Miller-Rabin Primality Test
bool millerRabinTest(BigInteger n, BigInteger d) {
	BigInteger a = BigInteger(2) + BigInteger(rand() / RAND_MAX) * ((n - 2) - 2);
	BigInteger x = pow(a, d, n);

	if (x == 1 || x == n - 1) {
		return true;
	}

	while (d != n - 1) {
		x = pow(x, 2, n);
		d *= 2;

		if (x == 1) {
			return false;
		}
		else if (x == n - 1) {
			return true;
		}
	}

	return false;
}

// basic primality test, falls back to Miller-Rabin
bool isPrime(BigInteger n) {
	if (n <= 1) {
		return false;
	}

	// n == 2 || n == 3
	if (n <= 3) {
		return true;
	}

	// prime numbers less than 1000
	int primes[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997 };

	for (int prime : primes) {
		if (n == prime) {
			return true;
		} else if (n % prime == 0) {
			// divisible by prime number
			return false;
		}
	}

	// find d such that d * 2^r = n - 1
	BigInteger d = n - 1; // n odd (b/c not divisible by 2) so d even
	while (d % 2 == 0) { // divide by 2 until d odd
		d /= 2;
	}

	// try to prove not prime 16 times
	int k = 16;
	for (int i = 0; i < k; i++) {
		if (!millerRabinTest(n, d)) {
			return false;
		}
	}

	// very likely prime
	return true;
}

int main() {
	std::cout << isPrime(stringToBigInteger("98764321267")) << std::endl;
}