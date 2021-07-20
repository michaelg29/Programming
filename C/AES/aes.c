#include "aes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char s_box[16][16] = {
    {	0x63,	0x7c,	0x77,	0x7b,	0xf2,	0x6b,	0x6f,	0xc5,	0x30,	0x01,	0x67,	0x2b,	0xfe,	0xd7,	0xab,	0x76    },
    {	0xca,	0x82,	0xc9,	0x7d,	0xfa,	0x59,	0x47,	0xf0,	0xad,	0xd4,	0xa2,	0xaf,	0x9c,	0xa4,	0x72,	0xc0    },
    {	0xb7,	0xfd,	0x93,	0x26,	0x36,	0x3f,	0xf7,	0xcc,	0x34,	0xa5,	0xe5,	0xf1,	0x71,	0xd8,	0x31,	0x15    },
    {	0x04,	0xc7,	0x23,	0xc3,	0x18,	0x96,	0x05,	0x9a,	0x07,	0x12,	0x80,	0xe2,	0xeb,	0x27,	0xb2,	0x75    },
    {	0x09,	0x83,	0x2c,	0x1a,	0x1b,	0x6e,	0x5a,	0xa0,	0x52,	0x3b,	0xd6,	0xb3,	0x29,	0xe3,	0x2f,	0x84    },
    {	0x53,	0xd1,	0x00,	0xed,	0x20,	0xfc,	0xb1,	0x5b,	0x6a,	0xcb,	0xbe,	0x39,	0x4a,	0x4c,	0x58,	0xcf    },
    {	0xd0,	0xef,	0xaa,	0xfb,	0x43,	0x4d,	0x33,	0x85,	0x45,	0xf9,	0x02,	0x7f,	0x50,	0x3c,	0x9f,	0xa8    },
    {	0x51,	0xa3,	0x40,	0x8f,	0x92,	0x9d,	0x38,	0xf5,	0xbc,	0xb6,	0xda,	0x21,	0x10,	0xff,	0xf3,	0xd2    },
    {	0xcd,	0x0c,	0x13,	0xec,	0x5f,	0x97,	0x44,	0x17,	0xc4,	0xa7,	0x7e,	0x3d,	0x64,	0x5d,	0x19,	0x73    },
    {	0x60,	0x81,	0x4f,	0xdc,	0x22,	0x2a,	0x90,	0x88,	0x46,	0xee,	0xb8,	0x14,	0xde,	0x5e,	0x0b,	0xdb    },
    {	0xe0,	0x32,	0x3a,	0x0a,	0x49,	0x06,	0x24,	0x5c,	0xc2,	0xd3,	0xac,	0x62,	0x91,	0x95,	0xe4,	0x79    },
    {	0xe7,	0xc8,	0x37,	0x6d,	0x8d,	0xd5,	0x4e,	0xa9,	0x6c,	0x56,	0xf4,	0xea,	0x65,	0x7a,	0xae,	0x08    },
    {	0xba,	0x78,	0x25,	0x2e,	0x1c,	0xa6,	0xb4,	0xc6,	0xe8,	0xdd,	0x74,	0x1f,	0x4b,	0xbd,	0x8b,	0x8a    },
    {	0x70,	0x3e,	0xb5,	0x66,	0x48,	0x03,	0xf6,	0x0e,	0x61,	0x35,	0x57,	0xb9,	0x86,	0xc1,	0x1d,	0x9e    },
    {	0xe1,	0xf8,	0x98,	0x11,	0x69,	0xd9,	0x8e,	0x94,	0x9b,	0x1e,	0x87,	0xe9,	0xce,	0x55,	0x28,	0xdf    },
    {	0x8c,	0xa1,	0x89,	0x0d,	0xbf,	0xe6,	0x42,	0x68,	0x41,	0x99,	0x2d,	0x0f,	0xb0,	0x54,	0xbb,	0x16    }
};

unsigned char mixColMat[BLOCK_SIDE][BLOCK_SIDE] = {
    { 0x02, 0x03, 0x01, 0x01 },
    { 0x01, 0x02, 0x03, 0x01 },
    { 0x01, 0x01, 0x02, 0x03 },
    { 0x03, 0x01, 0x01, 0x02 }
};

/*
    UTILITY METHODS
*/

// access value from the S-box
unsigned char getSub(unsigned char val) {
    // most significant nibble (x) is the row
    // least significant nibble (y) is the column
    unsigned char x = (val & 0xf0) >> 4;
    unsigned char y = (val & 0x0f);

    return s_box[x][y];
}

// reverse an array between the specified bounds
void reverseArray(unsigned char *arr, int start, int end) {
    while (start < end) {
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}

// left rotate an array d positions
void leftRotate(unsigned char *arr, int d, int len) {
    // rotate using the reversal algorithm
    if (!d) {
        return;
    }

    d %= len;

    reverseArray(arr, 0, d - 1);
    reverseArray(arr, d, len - 1);
    reverseArray(arr, 0, len - 1);
}

// perform Galois Field multiplication of two bytes in GF(2^8)
unsigned char gMul(unsigned char g1, unsigned char g2) {
    unsigned char p = 0;

    for (int i = 0; i < 8; i++) {
        if ((g2 & 1)) { // if LSB active
            p ^= g1;
        }

        bool MSB = (g1 & 0x80);
        g1 <<= 1; // rotate g1 left (multiply by x)
        if (MSB) { // if overflow, reduce
            g1 ^= 0x1B; // mod (x^8 + x^4 + x^3 + x + 1) = AES irreducible polynomial
        }
        g2 >>= 1; // rotate g2 right (divide by x)
    }

    return p;
}

/*
    AES ENCRYPTION LAYERS
*/

void addRoundKey(unsigned char state[BLOCK_SIDE][BLOCK_SIDE], unsigned char subkey[BLOCK_SIDE][BLOCK_SIDE]) {
    // add (bitwise XOR) in GF(2^8) corresponding bytes of the subkey and state
    for (int i = 0; i < BLOCK_SIDE; i++) {
        for (int j = 0; j < BLOCK_SIDE; j++) {
            state[i][j] ^= subkey[i][j];
        }
    }
}

void byteSub(unsigned char state[BLOCK_SIDE][BLOCK_SIDE]) {
    // substitute each byte in the state using the S-box
    for (int i = 0; i < BLOCK_SIDE; i++) {
        for (int j = 0; j < BLOCK_SIDE; j++) {
            state[i][j] = getSub(state[i][j]);
        }
    }
}

void shiftRows(unsigned char state[BLOCK_SIDE][BLOCK_SIDE]) {
    // rotate each row according to its position
    for (int i = 0; i < BLOCK_SIDE; i++) {
        leftRotate(state[i], i, BLOCK_SIDE);
    }
}

void mixCols(unsigned char state[BLOCK_SIDE][BLOCK_SIDE]) {
    unsigned char out[BLOCK_SIDE][BLOCK_SIDE];

    // matrix multiplication
    for (int r = 0; r < BLOCK_SIDE; r++) {
        for (int c = 0; c < BLOCK_SIDE; c++) {
            out[r][c] = 0x00;
            // dot product between the r-th row of mixColMat and the c-th col of state
            for (int i = 0; i < BLOCK_SIDE; i++) {
                out[r][c] ^= gMul(mixColMat[r][i], state[i][c]);
            }
        }
    }

    // copy memory to the state
    memcpy(state, out, BLOCK_SIDE * BLOCK_SIDE * sizeof(unsigned char));
}

void transformKey128(unsigned char key[BLOCK_SIDE][BLOCK_SIDE], unsigned char roundCoeff) {
    // first word in the subkey
    unsigned char g[4] = {
        getSub(key[1][3]) ^ roundCoeff, // bitwise XOR first byte with the round coefficient
        getSub(key[2][3]),
        getSub(key[3][3]),
        getSub(key[0][3])
    };

    for (int i = 0; i < BLOCK_SIDE; i++) {
        key[i][0] ^= g[i];
    }

    // second, third, fourth words in the subkey
    for (int c = 1; c < BLOCK_SIDE; c++) {
        for (int r = 0; r < BLOCK_SIDE; r++) {
            key[r][c] ^= key[r][c-1];
        }
    }
}

void aes_encrypt_block(unsigned char *in_text, int n, unsigned char in_key[BLOCK_LEN], unsigned char out[BLOCK_LEN]) {
    // represent the state and key as a 4x4 table (read into columns)
    unsigned char state[BLOCK_SIDE][BLOCK_SIDE];
    unsigned char key[BLOCK_SIDE][BLOCK_SIDE];

    int i = 0;
    for (int c = 0; c < BLOCK_SIDE; c++) {
        for (int r = 0; r < BLOCK_SIDE; r++) {
            state[r][c] = (i < n) ? in_text[i] : 0;
            key[r][c] = in_key[i];
            i++;
        }
    }

    // round coefficient for key transformation
    unsigned char roundCoeff = 0x01;

    // key addition layer
    addRoundKey(state, key);

    for (int i = 0; i < NR; i++) {
        // each step
        byteSub(state);
        shiftRows(state);

        if (i < NR - 1) {
            // do not mix columns in last row
            mixCols(state);
        }

        // key addition layer
        #if AES == 128
            transformKey128(key, roundCoeff);
        #endif
        roundCoeff = gMul(roundCoeff, 0x02); // multiply the round coefficient by x in GF(2^8)
        addRoundKey(state, key);
    }

    // copy bits of state into the output column by column
    i = 0;
    for (int c = 0; c < BLOCK_SIDE; c++) {
        for (int r = 0; r < BLOCK_SIDE; r++) {
            out[i++] = state[r][c];
        }
    }
}

void aes_encrypt(unsigned char *in_text, int n, unsigned char in_key[BLOCK_LEN], unsigned char *out) {
    // get number blocks
    int noBlocks = n >> 4;
    int extra = n & 0xf;
    
    // encrypt complete blocks
    for (int i = 0; i < noBlocks; i++) {
        aes_encrypt_block(in_text + (i << 4), BLOCK_LEN, in_key, out + (i << 4));
    }

    // encrypt any leftover text
    if (extra) {
        aes_encrypt_block(in_text + (noBlocks << 4), extra, in_key, out + (noBlocks << 4));
    }
}