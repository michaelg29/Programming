#include <stdio.h>
#include <stdlib.h>

#include "aes.h"

char hex[16] = "0123456789abcdef";
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
    for (int i = 0; i < len; i++) {
        printf("%c", arr[i]);
    }
    printf("\n");
}

void printHexString(unsigned char *arr, int len) {
    for (int i = 0; i < len; i++)
    {
        printf("%c%c ", hex[arr[i] >> 4], hex[arr[i] & 0x0f]);
    }
    printf("\n");
}

void printState(unsigned char a[4][4]) {
    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            printf("%c%c ", hex[a[r][c] >> 4], hex[a[r][c] & 0x0f]);
        }
    }
    printf("\n");
}

int main()
{
    printf("Hello, world!\n");

    unsigned char *txt = "asidlhgfyiuyguaysdgbagasdcvetweeqwert";
    unsigned char *key = "abcdefghijklmnop";

    unsigned char iv[16] = "zyxwvutsrqponmlk";
    //randomCharArray(iv, 16);

    unsigned char *cipher = NULL;
    unsigned char *text = NULL;

    int ctr_len = aes_encrypt(txt, 37, key, AES_128, AES_CTR, iv, &cipher);
    int txt_len = aes_decrypt(cipher, ctr_len, key, AES_128, AES_CTR, iv, &text);

    printf("Plaintext:     ");
    printString(txt, 37);
    printf("Key:           ");
    printHexString(key, 16);
    printf("IV:            ");
    printHexString(iv, 16);
    printf("CTR Cipher:    ");
    printHexString(cipher, ctr_len);
    printf("CTR Decrypted: ");
    printString(text, txt_len);

    // proof of inverse matrix
    // aes_invMixColMat will be the identity matrix (A .* A^-1 = I)
    //aes_mixCols(aes_invMixColMat);

    free(txt);
    free(key);
    free(text);
    free(cipher);

    return 0;
}

// 97 d9 a3 48 31 ae ed 33 d4 5e 0f d8 a4 c4 57 2d e0 7e d6 ce 40 dd 70 79 56 b4 b3 bd e9 22 39 59 aa 8a 61 e2 3b
// 97 d9 a3 48 31 ae ed 33 d4 5e 0f d8 a4 c4 57 2d ed 54 9f f0 58 eb e6 c2 c8 e3 35 4c 2c 15 0e f4 9d 2e 53 08 44