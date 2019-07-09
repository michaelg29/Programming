#include <iostream>

void keyExpansion() {}
void initialRound() {}

void subBytes() {}
void shiftRows() {}
void mixColumns() {}
void addRoundKey() {}

void encrypt() {
	int numberOfRounds = 1;

	keyExpansion();
	initialRound(); // whitening/add round key

	for (int i = 0; i < numberOfRounds; i++) {
		subBytes();
		shiftRows();
		mixColumns();
		addRoundKey();
	}

	// final round
	subBytes();
	shiftRows();
	addRoundKey();
}

int main() {
	std::cout << "AES Encryption" << std::endl;

	char message[] = "This is a message we will encrypt";
	char key[16] = {
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
	};

	encrypt();
}