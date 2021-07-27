#include <stdio.h>
#include <stdlib.h>

#include "aes.h"

char hex[16] = "0123456789ABCDEF";
void printCharArr(unsigned char *arr, int len, bool asChar)
{
    printf("{ ");
    for (int i = 0; i < len; i++)
    {
        printf("%c%c ", hex[arr[i] >> 4], hex[arr[i] & 0x0f]);
    }
    printf("}\n");
}

void printString(unsigned char *arr, int len) {
    for (int i = 0; i < len; i++)
    {
        printf("%c%c", hex[arr[i] >> 4], hex[arr[i] & 0x0f]);
    }
    printf("\n");
}

int main()
{
    printf("Hello, world!\n");

    unsigned char *txt = "asidlhgfyiuyguaysdgbagasdcvetwee";
    unsigned char *key = "abcdefghijklmnop";
    unsigned char *cipher = NULL;
    unsigned char *dec = NULL;

    int noBlocks = aes_encrypt(txt, 32, key, 16, &cipher);
    aes_decrypt(cipher, noBlocks, key, 16, &dec);

    printf("Plaintext: ");
    //printCharArr(txt, 32, false);
    printString(txt, 32);
    printf("Key: ");
    //printCharArr(key, 16, false);
    printString(key, 16);
    printf("Cipher: ");
    //printCharArr(cipher, 32, false);
    printString(cipher, 32);
    printf("Decrypted: ");
    //printCharArr(dec, 32, false);
    printString(dec, 32);

    // proof of inverse matrix
    // aes_invMixColMat will be the identity matrix (A .* A^-1 = I)
    //aes_mixCols(aes_invMixColMat);

    return 0;
}