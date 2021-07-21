typedef char bool;
#define false (bool)0
#define true (bool)1

#define BLOCK_LEN 16
#define BLOCK_SIDE 4

#define AES_128 128
#define AES_192 192
#define AES_256 256

#ifndef AES_H
#define AES_H

/*
    REFERENCE TABLES
*/

// substitution box
extern unsigned char s_box[256];
extern unsigned char inv_s_box[256];

// constant matrix for mix columns layer
extern unsigned char mixColMat[BLOCK_SIDE][BLOCK_SIDE];
extern unsigned char invMixColMat[BLOCK_SIDE][BLOCK_SIDE];

/*
    UTILITY METHODS
*/

void printCharArr(unsigned char *arr, int len, bool asChar);

// reverse an array between the specified bounds
void reverseArray(unsigned char *arr, int start, int end);

// left rotate an array d positions
void leftRotate(unsigned char *arr, int d, int len);
// right rotate an array d positions
void rightRotate(unsigned char *arr, int d, int len);

// perform Galois Field multiplication of two bytes in GF(2^8)
unsigned char gMul(unsigned char g1, unsigned char g2);

/*
    AES ENCRYPTION LAYERS
*/

void addRoundKey(unsigned char state[BLOCK_SIDE][BLOCK_SIDE], unsigned char subkey[BLOCK_SIDE][BLOCK_SIDE]);
void byteSub(unsigned char state[BLOCK_SIDE][BLOCK_SIDE]);
void shiftRows(unsigned char state[BLOCK_SIDE][BLOCK_SIDE]);
void mixCols(unsigned char state[BLOCK_SIDE][BLOCK_SIDE]);

void aes_encrypt_block(unsigned char *in_text, int n,
                    unsigned char subkeys[][BLOCK_SIDE][BLOCK_SIDE], int nr,
                    unsigned char out[BLOCK_LEN]);

int aes_encrypt(unsigned char *in_text, int n,
                unsigned char *in_key, int keylen,
                unsigned char **out);

/*
    AES DECRYPTION LAYERS
*/

void invMixCols(unsigned char state[BLOCK_SIDE][BLOCK_SIDE]);
void invShiftRows(unsigned char state[BLOCK_SIDE][BLOCK_SIDE]);
void invByteSub(unsigned char state[BLOCK_SIDE][BLOCK_SIDE]);

// void aes_decrypt_block(...);
// void aes_decrypt(...);

/*
    KEY SCHEDULE
*/

void generateKeySchedule(unsigned char *in_key, int keylen, unsigned char subkeys[][BLOCK_SIDE][BLOCK_SIDE]);
void generateKeySchedule128(unsigned char *in_key, unsigned char subkeys[11][BLOCK_SIDE][BLOCK_SIDE]);
void generateKeySchedule192(unsigned char *in_key, unsigned char subkeys[13][BLOCK_SIDE][BLOCK_SIDE]);
void generateKeySchedule256(unsigned char *in_key, unsigned char subkeys[15][BLOCK_SIDE][BLOCK_SIDE]);

#endif