#define AES 128

#include "aes.h"
#include <stdio.h>

char hex[16] = "0123456789ABCDEF";
void printCharArr(unsigned char *arr, int len, bool asChar) {
    printf("{ ");
    for (int i = 0; i < len; i++) {
        if (asChar) {
            printf("%c ", arr[i]);
        }
        else {
            printf("%c%c ", hex[arr[i] >> 4], hex[arr[i] & 0x0f]);
        }
    }
    printf("}\n");
}

int main() {
    unsigned char x[BLOCK_LEN] = "Two One Nine Two";
    unsigned char y[BLOCK_LEN];
    unsigned char k[BLOCK_LEN] = "Thats my Kung Fu";

    aes_encrypt(x, BLOCK_LEN, k, y);

    printCharArr(x, BLOCK_LEN, false);
    printCharArr(k, BLOCK_LEN, false);
    printCharArr(y, BLOCK_LEN, false);

    return 0;
}