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
    for (int i = 0; i < len; i++) {
        printf("%c", arr[i]);
    }
    printf("\n");
}

void printHexString(unsigned char *arr, int len) {
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
    unsigned char *ecb_cipher = NULL;
    unsigned char *cbc_cipher = NULL;
    unsigned char *ecb_dec = NULL;
    unsigned char *cbc_dec = NULL;

    unsigned char iv[16] = "zyxwvutsrqponmlk";
    //randomCharArray(iv, 16);

    int ecb_noBlocks = aes_encrypt(txt, 37, key, 16, AES_ECB, NULL, &ecb_cipher);
    int ecb_len = aes_decrypt(ecb_cipher, ecb_noBlocks, key, 16, AES_ECB, NULL, &ecb_dec);

    int cbc_noBlocks = aes_encrypt(txt, 37, key, 16, AES_CBC, iv, &cbc_cipher);
    int cbc_len = aes_decrypt(cbc_cipher, cbc_noBlocks, key, 16, AES_CBC, iv, &cbc_dec);

    printf("Plaintext:     ");
    printString(txt, 37);
    printf("Key:           ");
    printString(key, 16);
    printf("ECB Cipher:    ");
    printHexString(ecb_cipher, ecb_noBlocks * BLOCK_LEN);
    printf("ECB Decrypted: ");
    printString(ecb_dec, ecb_len);
    printf("IV:            ");
    printString(iv, 16);
    printf("CBC Cipher:    ");
    printHexString(cbc_cipher, cbc_noBlocks * BLOCK_LEN);
    printf("CBC Decrypted: ");
    printString(cbc_dec, cbc_len);

    // proof of inverse matrix
    // aes_invMixColMat will be the identity matrix (A .* A^-1 = I)
    //aes_mixCols(aes_invMixColMat);

    free(txt);
    free(key);
    free(ecb_cipher);
    free(cbc_cipher);
    free(ecb_dec);
    free(cbc_dec);

    return 0;
}

// 59A2F2C752C26C5C0F28A7705E0247B42CCD7BF2A207C3CD7BA0844F75E2E03ABCBCF15DA39F37129A05AA50DEFAA3A5
// 59A2F2C752C26C5C0F28A7705E0247B42CCD7BF2A207C3CD7BA0844F75E2E03ABCBCF15DA39F37129A05AA50DEFAA3A5