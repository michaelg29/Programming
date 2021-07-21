#include "aes.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    // unsigned char x[BLOCK_LEN] = "Two One Nine Two";
    // unsigned char y[BLOCK_LEN]; // { 29 C3 50 5F 57 14 20 F6 40 22 99 B3 1A 02 D7 3A }
    // unsigned char k[BLOCK_LEN] = "Thats my Kung Fu";

    //https://csrc.nist.gov/csrc/media/projects/cryptographic-algorithm-validation-program/documents/aes/aesavs.pdf

    unsigned char x[16] = "Two One Nine Two";
    unsigned char *y = NULL;
    unsigned char k[BLOCK_LEN] = "Thats my Kung Fu";

    int n = aes_encrypt(x, 16, k, 128, &y);

    printf("=======\n");
    printf("Plaintext: ");
    printCharArr(x, 16, false);
    printf("Key: ");
    printCharArr(k, BLOCK_LEN, false);
    printf("Encrypted: ");
    printCharArr(y, 16, false);   

    return 0;
}