#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>

#include <BigIntegerLibrary.hh>
#include <BigInteger.hh>

std::vector<std::string> split(const char in[]);
BigInteger pow(BigInteger x, BigInteger y, BigInteger p);
bool millerRabinTest(BigInteger n, BigInteger d);
bool isPrime(BigInteger n);
BigInteger gcd(BigInteger p, BigInteger q);
BigInteger egcd(BigInteger a, BigInteger b, BigInteger* x, BigInteger* y);
bool isCoPrime(BigInteger p, BigInteger q);
BigInteger generateLargeNum(int keysize = 1024);
BigInteger generateLargePrime(int keysize = 1024);
BigInteger modularInverse(BigInteger a, BigInteger m);
BigInteger* generateKeys(int keysize = 1024);
std::string encrypt(BigInteger e, BigInteger n, std::string msg);
std::string decrypt(BigInteger d, BigInteger n, std::string cypher);

BigUnsigned bintmax;

int main() {
	int keysize = 32;
	for (int i = 0; i <= keysize; i++) {
		bintmax.setBit(i, true);
	}

	BigInteger* keys = generateKeys(keysize);
	BigInteger e = keys[0];
	BigInteger d = keys[1];
	BigInteger n = keys[2];

	std::string msg = "Hello, world!";

	std::string enc = encrypt(e, n, msg);
	std::string dec = decrypt(d, n, enc);

	std::cout << "Message: " << msg << std::endl;
	std::cout << "e: " << e << std::endl;
	std::cout << "d: " << d << std::endl;
	std::cout << "N: " << n << std::endl;
	std::cout << "enc: " << enc << std::endl;
	std::cout << "dec: " << dec << std::endl;

	return 0;
}

std::vector<std::string> split(const char in[]) {
	std::istringstream iss(in);
	std::istream_iterator<std::string> first(iss), last;

	std::vector<std::string> parts;
	std::copy(first, last, std::back_inserter(parts));
	return parts;
}

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
		if (!millerRabinTest((BigInteger)n, d)) {
			return false;
		}
	}

	// very likely prime
	return true;
}

BigInteger gcd(BigInteger p, BigInteger q) {
	// euclidean algorithm

	if (p == 0) {
		return q;
	}

	return gcd(p % q, q);
	return 0;
}

BigInteger egcd(BigInteger a, BigInteger b, BigInteger *x, BigInteger *y) {
	if (a == 0) {
		*x = 0;
		*y = 1;
		return b;
	}

	BigInteger x1, y1;
	BigInteger gcd = egcd(b % a, a, &x1, &y1);

	*x = y1 - (b / a) * x1;
	*y = x1;

	return gcd;
}

bool isCoPrime(BigInteger p, BigInteger q) {
	return gcd(p, q) == 1;
}

BigInteger generateLargeNum(int keysize) {
	//BigInteger a = BigInteger(2) + BigInteger(rand() / RAND_MAX) * ((n - 2) - 2);

	BigInteger lower = pow(2, keysize - 1, bintmax);
	BigInteger upper = pow(2, keysize, bintmax);

	return lower + BigInteger(rand() / RAND_MAX) * (upper - lower);
}

BigInteger generateLargePrime(int keysize) {
	BigInteger ret;

	while (true) {
		ret = generateLargeNum(keysize);
	}

	return ret;
}

BigInteger modularInverse(BigInteger a, BigInteger m) {
	BigInteger x, y;
	egcd(a, m, &x, &y);

	if (x < 0) {
		x += m;
	}

	return x;
}

BigInteger* generateKeys(int keysize) {
	BigInteger e;
	BigInteger d;
	BigInteger n;

	//BigInteger p = generateLargePrime(keysize);
	//BigInteger q = generateLargePrime(keysize);
	BigInteger p = 7;
	BigInteger q = 17;

	n = p * q;

	BigInteger phiN = (p - 1) * (q - 1);

	// choose e, which is coprime with phiN
	while (true) {
		e = generateLargeNum(keysize);
		if (isCoPrime(e, phiN)) {
			break;
		}
	}

	d = modularInverse(e, phiN);

	BigInteger* ret = new BigInteger[3];
	ret[0] = e;
	ret[1] = d;
	ret[2] = n;

	return ret;
}

std::string encrypt(BigInteger e, BigInteger n, std::string msg) {
	std::stringstream cipher;

	for (char c : msg) {
		cipher << pow(c, e, n) << ' ';
	}

	return cipher.str();
}

std::string decrypt(BigInteger d, BigInteger n, std::string cipher) {
	std::stringstream text;
	std::vector<std::string> parts = split(cipher.c_str());
	for (std::string part : parts) {
		if (part != "") {
			std::string dec = bigIntegerToString(pow(std::stoi(part), d, n));
			text << char(std::stoi(dec));
		}
	}

	return text.str();
}