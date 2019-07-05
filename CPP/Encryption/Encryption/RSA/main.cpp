#include <string>
#include <iostream>
#include <map>
#include <list>

std::string chars = R"(0123456789!@#$%^&*()-=_+qwertyuiopasdfghjklzxcvbnm QWERTYUIOPASDFGHJKLZXCVBNM[]{}\|;':\",./<>?)";
std::map<char, int> dict;

std::list<std::string> split(std::string, char split) {

}

bool isPrime(int n) {
	if (n <= 1)
		return false;

	// if n is even or 3
	if (n % 2 == 0 || n == 3)
		return true;

	// test each odd number from 3 to sqrt(n)
	for (int i = 3; i < sqrt(n) + 1; i += 2)
		if (n % i == 0)
			return false;

	// n is probably prime
	return true;
}

int gcd(int p, int q) {
	while (q) {
		int temp = q;
		q = p % q;
		p = temp;
	}

	return p;
}

bool isCoPrime(int p, int q) {
	return gcd(p, q) == 1;
}

std::string encrypt(int e, int n, std::string msg) {
	std::string ret = "";
	for (char c : msg)
		ret += int(pow(dict[c], e)) % n + " ";
	return ret;
}

std::string decrypt(int d, int n, std::string cyp) {
	std::string ret = "";



	return ret;
}

int main() {
	for (int i = 0; i < chars.length(); i++)
		dict[chars[i]] = i;


	std::cout << gcd(9, 12);
}