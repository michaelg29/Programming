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

    unsigned char *txt = "asidlhgfyiuyguaysdgbagasdcvetweeqwert";
    unsigned char *key = "abcdefghijklmnop";
    unsigned char *cipher = NULL;
    unsigned char *dec = NULL;

    int noBlocks = aes_encrypt(txt, 37, key, 16, &cipher);
    aes_decrypt(cipher, noBlocks, key, 16, &dec);
    int padLength = dec[noBlocks * BLOCK_LEN - 1];

    printf("Plaintext: ");
    //printCharArr(txt, 32, false);
    printString(txt, 37);
    printf("Key: ");
    //printCharArr(key, 16, false);
    printString(key, 16);
    printf("Cipher: ");
    //printCharArr(cipher, 32, false);
    printString(cipher, noBlocks * BLOCK_LEN);
    printf("Decrypted: ");
    //printCharArr(dec, 32, false);
    printString(dec, noBlocks * BLOCK_LEN - padLength);

    // proof of inverse matrix
    // aes_invMixColMat will be the identity matrix (A .* A^-1 = I)
    //aes_mixCols(aes_invMixColMat);

    free(txt);
    free(key);
    free(cipher);
    free(dec);

    return 0;
}

// 3B752E0772CE3B300FEE15DDC8F7191F99B5A4EFD9F02DC02A5C2B57C71506A4F23D135CD9B460AC0100F93D3937C410
// 3B752E0772CE3B300FEE15DDC8F7191F99B5A4EFD9F02DC02A5C2B57C71506A48E64CE873F174DBB2423FCD814580E15