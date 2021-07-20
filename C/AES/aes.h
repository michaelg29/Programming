typedef char bool;
#define false (bool)0
#define true (bool)1

#define BLOCK_LEN 16
#define BLOCK_SIDE 4

#if defined(AES_192)
    #define AES 192
    #define NR 12
#elif defined(AES_256)
    #define AES 256
    #define NR 14
#else
    // default
    #define AES 128
    #define NR 10
#endif

#ifndef AES_H
#define AES_H

/*
    REFERENCE TABLES
*/

// substitution box
extern unsigned char s_box[16][16];

// constant matrix for mix columns layer
extern unsigned char mixColMat[BLOCK_SIDE][BLOCK_SIDE];

/*
    UTILITY METHODS
*/

// access value from the S-box
unsigned char getSub(unsigned char val);

// reverse an array between the specified bounds
void reverseArray(unsigned char *arr, int start, int end);

// left rotate an array d positions
void leftRotate(unsigned char *arr, int d, int len);

// perform Galois Field multiplication of two bytes in GF(2^8)
unsigned char gMul(unsigned char g1, unsigned char g2);

/*
    AES ENCRYPTION LAYERS
*/

void addRoundKey(unsigned char state[BLOCK_SIDE][BLOCK_SIDE], unsigned char subkey[BLOCK_SIDE][BLOCK_SIDE]);
void byteSub(unsigned char state[BLOCK_SIDE][BLOCK_SIDE]);
void shiftRows(unsigned char state[BLOCK_SIDE][BLOCK_SIDE]);
void mixCols(unsigned char state[BLOCK_SIDE][BLOCK_SIDE]);
void transformKey128(unsigned char key[BLOCK_SIDE][BLOCK_SIDE], unsigned char roundCoeff);
void aes_encrypt_block(unsigned char *in_text, int n, unsigned char in_key[BLOCK_LEN], unsigned char out[BLOCK_LEN]);
void aes_encrypt(unsigned char *in_text, int n, unsigned char in_key[BLOCK_LEN], unsigned char *out);

#endif