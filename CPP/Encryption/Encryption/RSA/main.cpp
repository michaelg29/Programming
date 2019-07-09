#include <string>
#include <cmath>
#include <iostream>
#include <vector>
#include <sstream>

/*
c++ algorithm to deal with rsa asymmetric key encryption/decryption
*/

std::vector<std::string> split(std::string str, char regex) {
	std::vector<std::string> ret;

	std::string item;

	for (char c : str) {
		if (c == regex) {
			ret.push_back(item);
			item = "";
		} else {
			item += c;
		}
	}

	ret.push_back(item);

	return ret;
}

bool rabinMiller(int n) {
	/*
		return true if n is prime
	*/

	int s = n - 1;
	int t = 0;

	while (s % 2 == 0) {
		// count number to times to halve n
		s = (int)(s / 2);
		t++;
	}

	// try to prove not prime five times
	for (int i = 0; i < 128; i++) {
		int a = 2 + (n - 3) * rand();
		int v = (int)pow(a, s) % n;

		if (v != 1) {
			// test does not apply if v == 1
			int i = 0;
			while (v != n - 1) {
				if (i == t - 1)
					return false;
				else {
					i++;
					v = (int)pow(v, 2) % n;
				}
			}
		}
	}

	// n is very likely prime
	return true;
}

bool isPrime(int n) {
	/*
		return true if n is prime
        resorts to rabinMiller algorithm if uncertain
	*/

	// 0, 1, -ve numbers not prime
	if (n < 2) return false;

	int lowPrimes[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997 };

	for (int prime : lowPrimes)
		if (prime == n || n % prime == 0)
			return false;

	// else do rabin-miller
	return rabinMiller(n);
}

int gcd(int p, int q) {
	/*
		euclidean algorithm to determine greatest common divisor of p and q
	*/

	while (q != 0) {
		int temp = q;
		q = p % q;
		p = temp;
	}

	return p;
}

int* egcd(int p, int q) {
	/*
		euclids extended greatest common divisor algorithm
        ax + by = gcd(a, b)
        returns list, result, of size 3:
            result[0] = gcd(a, b)
            result[1] = x
            result[2] = y
	*/

	int s = 0, old_s = 1,
		t = 1, old_t = 0,
		r = q, old_r = p;

	while (r != 0) {
		int quotient = (int)(old_r / r);
		
		int temp = old_r;
		old_r = r;
		r = temp - quotient * r;

		temp = old_s;
		old_s = s;
		s = temp - quotient * s;

		temp = old_t;
		old_t = t;
		t = temp - quotient * t;

		int ret[] = { old_r, old_s, old_t };

		return ret;
	}

	int ret[] = { 1, 1, 1 };

	return ret;
}

bool isCoPrime(int p, int q) {
	/*
		return True if p and q are coPrime
        if gcd(p, q) == 1
	*/

	return gcd(p, q) == 1;
}

long generateLargePrime(int keysize = 1024) {
	/*
		return a random large prime number of keysize bits in size
	*/

	int min = (int)pow(2, keysize - 1),
		max = (int)pow(2, keysize);

	long num;
	while (true) {
		num = min + (max - min) * rand();
		if (isPrime(num))
			return num;
	}
}

int modularInverse(int a, int m) {
	/*
		return modular multiplicative inverse of a and m
        return -1 if dne
	*/

	// gcd using ecluid's algorithm
	int* ret = egcd(a, m);
	if (ret[1] < 0)
		ret[1] += m;

	return ret[1];
}

int* generateKeys(int keysize = 1024) {
	/*
		generates encryption and decryption keys of size keysize bits
	*/

	int min = (int)pow(2, keysize - 1),
		max = (int)pow(2, keysize);

	long e = 0,
		d = 0,
		N = 0;

	long p = generateLargePrime();
	long q = generateLargePrime();

	N = p * q;

	long phiN = (p - 1) * (q - 1); // number of co-prime numbers 1 <= x <= N

	// choose e (encryption key)
	// e is coprime with phiN
	while (true) {
		e = min + (max - min) * rand();
		if (isCoPrime(e, phiN))
			break;
	}

	// choose d (decryption key)
	// modular inverse of e in range phiN
	d = modularInverse(e, phiN);

	int ret[] = { e, d, N };

	return ret;
}

std::string encrypt(int e, int n, std::string msg) {
	/*
		encrypts msg using encryption key e
	*/

	std::stringstream cypher;

	for (char c : msg) {
		cypher << (int)pow((int)c, e) % n << ' ';
	}

	return cypher.str();
}

std::string decrypt(int d, int n, std::string cypher) {
	/*
		decrypts cypher using decryption key d
	*/

	std::vector<std::string> parts = split(cypher, ' ');
	std::string msg = "";

	for (std::string part : parts)
		if (part != "") {
			char c = '0' + std::stoi(part);
			msg += (char)('0' + std::stoi(part));
		}
			

	return msg;
}

int main() {
	int* keys = generateKeys();

	std::cout << "Encryption key: (" << keys[0] << ", " << keys[2] << ")" << std::endl;
	std::cout << "Decryption key: (" << keys[1] << ", " << keys[2] << ")" << std::endl;

	std::string msg = "Hello, world!";
	std::string cypher = encrypt(keys[0], keys[2], msg);

	std::cout << "Message: " << msg << std::endl;
	std::cout << "Encrypted: " << cypher << std::endl;
	std::cout << "Decrypted: " << decrypt(keys[1], keys[2], cypher) << std::endl;
}