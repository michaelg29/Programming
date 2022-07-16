#include "aes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
    REFERENCE TABLES
*/

// substitution box
unsigned char aes_s_box[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};

unsigned char aes_inv_s_box[256] = {
    0x52,	0x09,	0x6a,	0xd5,	0x30,	0x36,	0xa5,	0x38,	0xbf,   0x40,	0xa3,	0x9e,	0x81,	0xf3,	0xd7,	0xfb,
    0x7c,	0xe3,	0x39,	0x82,	0x9b,	0x2f,	0xff,	0x87,	0x34,   0x8e,	0x43,	0x44,	0xc4,	0xde,	0xe9,	0xcb,
    0x54,	0x7b,	0x94,	0x32,	0xa6,	0xc2,	0x23,	0x3d,	0xee,   0x4c,	0x95,	0x0b,	0x42,	0xfa,	0xc3,	0x4e,
    0x08,	0x2e,	0xa1,	0x66,	0x28,	0xd9,	0x24,	0xb2,	0x76,   0x5b,	0xa2,	0x49,	0x6d,	0x8b,	0xd1,	0x25,
    0x72,	0xf8,	0xf6,	0x64,	0x86,	0x68,	0x98,	0x16,	0xd4,   0xa4,	0x5c,	0xcc,	0x5d,	0x65,	0xb6,	0x92,
    0x6c,	0x70,	0x48,	0x50,	0xfd,	0xed,	0xb9,	0xda,	0x5e,   0x15,	0x46,	0x57,	0xa7,	0x8d,	0x9d,	0x84,
    0x90,	0xd8,	0xab,	0x00,	0x8c,	0xbc,	0xd3,	0x0a,	0xf7,   0xe4,	0x58,	0x05,	0xb8,	0xb3,	0x45,	0x06,
    0xd0,	0x2c,	0x1e,	0x8f,	0xca,	0x3f,	0x0f,	0x02,	0xc1,   0xaf,	0xbd,	0x03,	0x01,	0x13,	0x8a,	0x6b,
    0x3a,	0x91,	0x11,	0x41,	0x4f,	0x67,	0xdc,	0xea,	0x97,   0xf2,	0xcf,	0xce,	0xf0,	0xb4,	0xe6,	0x73,
    0x96,	0xac,	0x74,	0x22,	0xe7,	0xad,	0x35,	0x85,	0xe2,   0xf9,	0x37,	0xe8,	0x1c,	0x75,	0xdf,	0x6e,
    0x47,	0xf1,	0x1a,	0x71,	0x1d,	0x29,	0xc5,	0x89,	0x6f,   0xb7,	0x62,	0x0e,	0xaa,	0x18,	0xbe,	0x1b,
    0xfc,	0x56,	0x3e,	0x4b,	0xc6,	0xd2,	0x79,	0x20,	0x9a,   0xdb,	0xc0,	0xfe,	0x78,	0xcd,	0x5a,	0xf4,
    0x1f,	0xdd,	0xa8,	0x33,	0x88,	0x07,	0xc7,	0x31,	0xb1,   0x12,	0x10,	0x59,	0x27,	0x80,	0xec,	0x5f,
    0x60,	0x51,	0x7f,	0xa9,	0x19,	0xb5,	0x4a,	0x0d,	0x2d,   0xe5,	0x7a,	0x9f,	0x93,	0xc9,	0x9c,	0xef,
    0xa0,	0xe0,	0x3b,	0x4d,	0xae,	0x2a,	0xf5,	0xb0,	0xc8,   0xeb,	0xbb,	0x3c,	0x83,	0x53,	0x99,	0x61,
    0x17,	0x2b,	0x04,	0x7e,	0xba,	0x77,	0xd6,	0x26,	0xe1,   0x69,	0x14,	0x63,	0x55,	0x21,	0x0c,	0x7d
};

// constant matrix for mix columns
unsigned char aes_mixColMat[BLOCK_SIDE][BLOCK_SIDE] = {
    {0x02, 0x03, 0x01, 0x01},
    {0x01, 0x02, 0x03, 0x01},
    {0x01, 0x01, 0x02, 0x03},
    {0x03, 0x01, 0x01, 0x02}};

unsigned char aes_invMixColMat[BLOCK_SIDE][BLOCK_SIDE] = {
    { 0x0E, 0x0B, 0x0D, 0x09 },
    { 0x09, 0x0E, 0x0B, 0x0D },
    { 0x0D, 0x09, 0x0E, 0x0B },
    { 0x0B, 0x0D, 0x09, 0x0E }
};

/*
    UTILITY METHODS
*/

// method to reverse an array between two bounds
void reverseArray(unsigned char *arr, int i1, int i2)
{
    while (i1 < i2)
    {
        // swap corresponding elements across the middle
        unsigned char tmp = arr[i1];
        arr[i1] = arr[i2];
        arr[i2] = tmp;
        i1++;
        i2--;
    }
}

// method to rotate an array to the left using the reversal algorithm
void leftRotate(unsigned char *arr, int d, int n)
{
    reverseArray(arr, 0, d - 1);
    reverseArray(arr, d, n - 1);
    reverseArray(arr, 0, n - 1);
}

// method to rotate an array to the right using the reversal algorithm
void rightRotate(unsigned char *arr, int d, int n)
{
    // complementary left rotation
    leftRotate(arr, n - d, n);
}

// perform Galois Field multiplication of two bytes in GF(2^8)
unsigned char galoisMul(unsigned char g1, unsigned char g2)
{
    // taken and documented from https://en.wikipedia.org/wiki/Rijndael_MixColumns
    unsigned char p = 0;

    for (int i = 0; i < 8; i++)
    {
        if (g2 & 0x01) // if LSB is active (equivalent to a '1' in the polynomial of g2)
        {
            p ^= g1; // p += g1 in GF(2^8)
        }

        bool hiBit = (g1 & 0x80); // g1 >= 128 = 1000 0000
        g1 <<= 1;                 // rotate g1 left (multiply by x in GF(2^8))
        if (hiBit)
        {
            // must reduce
            g1 ^= AES_IRREDUCIBLE; // g1 -= 00011011 == mod(x^8 + x^4 + x^3 + x + 1) = AES irreducible
        }
        g2 >>= 1; // rotate g2 right (divide by x in GF(2^8))
    }

    return p;
}

// generate a random array
void randomCharArray(unsigned char *arr, int n) {
    srand(time(0));
    for (int i = 0; i < n; i++) {
        arr[i] = rand() & 0xff;
    }
}

/*
    AES ENCRYPTION LAYERS
*/

void aes_addRoundKey(unsigned char state[BLOCK_SIDE][BLOCK_SIDE], unsigned char subkey[BLOCK_SIDE][BLOCK_SIDE])
{
    // add in GF(2^8) corresponding bytes of the subkey and state
    for (int r = 0; r < BLOCK_SIDE; r++)
    {
        for (int c = 0; c < BLOCK_SIDE; c++)
        {
            state[r][c] ^= subkey[r][c];
        }
    }
}

void aes_byteSub(unsigned char state[BLOCK_SIDE][BLOCK_SIDE])
{
    // substitute each byte using the s-box
    for (int r = 0; r < BLOCK_SIDE; r++)
    {
        for (int c = 0; c < BLOCK_SIDE; c++)
        {
            state[r][c] = aes_s_box[state[r][c]];
        }
    }
}

void aes_shiftRows(unsigned char state[BLOCK_SIDE][BLOCK_SIDE])
{
    // rotate each row according to its position
    for (int r = 0; r < BLOCK_SIDE; r++)
    {
        leftRotate(state[r], r, BLOCK_SIDE);
    }
}

void aes_mixCols(unsigned char state[BLOCK_SIDE][BLOCK_SIDE])
{
    unsigned char out[BLOCK_SIDE][BLOCK_SIDE];

    // matrix multiplication in GF(2^8)
    // * => galoisMul, + => ^
    for (int r = 0; r < BLOCK_SIDE; r++)
    {
        for (int c = 0; c < BLOCK_SIDE; c++)
        {
            out[r][c] = 0x00;
            // dot product of the r-th row of mixColMat and the c-th col of state
            for (int i = 0; i < BLOCK_SIDE; i++)
            {
                out[r][c] ^= galoisMul(aes_mixColMat[r][i], state[i][c]);
            }
        }
    }

    // copy memory to the state
    memcpy(state, out, BLOCK_SIDE * BLOCK_SIDE * sizeof(unsigned char));
}

void aes_encrypt_block(unsigned char *in_text, int n,
                       unsigned char subkeys[][BLOCK_SIDE][BLOCK_SIDE], int nr,
                       unsigned char iv[16],
                       unsigned char out[BLOCK_LEN])
{
    // represent the state and key as a 4x4 table (read into columns)
    unsigned char state[BLOCK_SIDE][BLOCK_SIDE];
    int i = 0;
    for (int c = 0; c < BLOCK_SIDE; c++)
    {
        for (int r = 0; r < BLOCK_SIDE; r++)
        {
            // use PKCS5 padding
            state[r][c] = (i < n) ?
                                in_text[i] :
                                BLOCK_LEN - n;

            if (iv) {
                // XOR with IV
                state[r][c] ^= iv[i];
            }

            i++;
        }
    }

    // ROUND 0
    aes_addRoundKey(state, subkeys[0]);

    // ROUNDS 1 --> NR-1
    for (i = 1; i < nr; i++)
    {
        aes_byteSub(state);
        aes_shiftRows(state);
        aes_mixCols(state);
        aes_addRoundKey(state, subkeys[i]);
    }

    // ROUND NR
    aes_byteSub(state);
    aes_shiftRows(state);
    aes_addRoundKey(state, subkeys[nr]);

    // copy bytes of state into the output column by column
    i = 0;
    for (int c = 0; c < BLOCK_SIDE; c++)
    {
        for (int r = 0; r < BLOCK_SIDE; r++)
        {
            out[i++] = state[r][c];
        }
    }
}

int aes_encrypt(unsigned char *in_text, int n,
                unsigned char *in_key, int keylen,
                unsigned char mode,
                unsigned char iv[16],
                unsigned char **out)
{
    // determine number of rounds
    int nr = 10; // AES_128 by default
    switch (keylen)
    {
    case AES_192:
        nr = 12;
        break;
    case AES_256:
        nr = 14;
        break;
    };

    // generate key schedule
    unsigned char(*subkeys)[BLOCK_SIDE][BLOCK_SIDE] =
        malloc((nr + 1) * sizeof(unsigned char[BLOCK_SIDE][BLOCK_SIDE]));
    aes_generateKeySchedule(in_key, keylen, subkeys);

    int noBlocks = n >> 4; // n / BLOCK_LEN
    int extra = n & 0x0f;  // n % BLOCK_LEN

    // allocate output
    int outLen = (noBlocks + 1) * BLOCK_LEN;
    *out = malloc(outLen * sizeof(unsigned char));

    // encrypt blocks (last one is extra/dummy)
    for (int i = 0; i <= noBlocks; i++)
    {
        int len = (i < noBlocks) ?
                    BLOCK_LEN :
                    extra;
        switch (mode) {
            case AES_CBC:
                if (!i) {
                    // use passed in iv for first block
                    aes_encrypt_block(in_text + (i << 4), len, subkeys, nr, iv, *out + (i << 4));
                }
                else {
                    // use previous encrypted block as iv
                    aes_encrypt_block(in_text + (i << 4), len, subkeys, nr, *out + ((i - 1) << 4), *out + (i << 4));
                }

                break;
            default: // AES_ECB
                aes_encrypt_block(in_text + (i << 4), len, subkeys, nr, NULL, *out + (i << 4));
                break;
        }
    }

    free(subkeys);

    return noBlocks + 1;
}

/*
    AES DECRYPTION LAYERS
*/

void aes_invAddRoundKey(unsigned char state[BLOCK_SIDE][BLOCK_SIDE], unsigned char subkey[BLOCK_SIDE][BLOCK_SIDE]) {
    aes_addRoundKey(state, subkey);
}

void aes_invByteSub(unsigned char state[BLOCK_SIDE][BLOCK_SIDE]) {
    // substitute each byte in the state using the inverse S-box
    for (int i = 0; i < BLOCK_SIDE; i++) {
        for (int j = 0; j < BLOCK_SIDE; j++) {
            state[i][j] = aes_inv_s_box[state[i][j]];
        }
    }
}

void aes_invShiftRows(unsigned char state[BLOCK_SIDE][BLOCK_SIDE]) {
    // rotate each row according to its position
    for (int i = 0; i < BLOCK_SIDE; i++) {
        rightRotate(state[i], i, BLOCK_SIDE);
    }
}

void aes_invMixCols(unsigned char state[BLOCK_SIDE][BLOCK_SIDE]) {
    unsigned char out[BLOCK_SIDE][BLOCK_SIDE];

    // matrix multiplication
    for (int r = 0; r < BLOCK_SIDE; r++) {
        for (int c = 0; c < BLOCK_SIDE; c++) {
            out[r][c] = 0x00;
            // dot product between the r-th row of invMixColMat and the c-th col of state
            for (int i = 0; i < BLOCK_SIDE; i++) {
                out[r][c] ^= galoisMul(aes_invMixColMat[r][i], state[i][c]);
            }
        }
    }

    // copy memory to the state
    memcpy(state, out, BLOCK_SIDE * BLOCK_SIDE * sizeof(unsigned char));
}

void aes_decrypt_block(unsigned char *in_cipher,
                    unsigned char subkeys[][BLOCK_SIDE][BLOCK_SIDE], int nr,
                    unsigned char iv[16],
                    unsigned char out[BLOCK_LEN]) {
    unsigned char state[BLOCK_SIDE][BLOCK_SIDE];

    int i = 0;
    for (int c = 0; c < BLOCK_SIDE; c++) {
        for (int r = 0; r < BLOCK_SIDE; r++) {
            state[r][c] = in_cipher[i];
            i++;
        }
    }

    // INVERSE ROUND NR
    aes_invAddRoundKey(state, subkeys[nr]);
    aes_invShiftRows(state);
    aes_invByteSub(state);

    // INVERSE ROUNDS NR-1 --> 1
    for (i = nr - 1; i > 0; i--) {
        aes_invAddRoundKey(state, subkeys[i]);
        aes_invMixCols(state);
        aes_invShiftRows(state);
        aes_invByteSub(state);
    }

    // INVERSE ROUND 0
    aes_invAddRoundKey(state, subkeys[0]);

    // copy bytes of state into the output column by column
    i = 0;
    for (int c = 0; c < BLOCK_SIDE; c++)
    {
        for (int r = 0; r < BLOCK_SIDE; r++)
        {
            out[i] = state[r][c];

            if (iv) {
                // XOR with IV
                out[i] ^= iv[i];
            }

            i++;
        }
    }
}

int aes_decrypt(unsigned char *in_cipher, int noBlocks,
                    unsigned char *in_key, int keylen,
                    unsigned char mode,
                    unsigned char iv[16],
                    unsigned char **out) {
    // determine number of rounds
    int nr = 10; // AES_128 by default
    switch (keylen)
    {
    case AES_192:
        nr = 12;
        break;
    case AES_256:
        nr = 14;
        break;
    };

    // generate key schedule
    unsigned char(*subkeys)[BLOCK_SIDE][BLOCK_SIDE] =
        malloc((nr + 1) * sizeof(unsigned char[BLOCK_SIDE][BLOCK_SIDE]));
    aes_generateKeySchedule(in_key, keylen, subkeys);

    unsigned char *paddedOut = malloc(noBlocks * BLOCK_LEN * sizeof(unsigned char));
    
    for (int i = noBlocks - 1; i >= 0; i--) {
        switch (mode) {
            case AES_CBC:
                if (!i) {
                    // use passed in iv for first block
                }
        }
    }
    
    for (int i = 0; i < noBlocks; i++) {
        switch (mode) {
            case AES_CBC:
                if (!i) {
                    // use passed in iv for first block
                    aes_decrypt_block(in_cipher + (i << 4), subkeys, nr, iv, paddedOut + (i << 4));
                }
                else {
                    // use previous block of input as iv
                    aes_decrypt_block(in_cipher + (i << 4), subkeys, nr, in_cipher + ((i - 1) << 4), paddedOut + (i << 4));
                }

                break;
            default: // AES_ECB
                aes_decrypt_block(in_cipher + (i << 4), subkeys, nr, NULL, paddedOut + (i << 4));
        }
    }

    free(subkeys);

    // reallocate to account for padding
    unsigned char noPadding = paddedOut[noBlocks * BLOCK_LEN - 1];

    *out = malloc((noBlocks * BLOCK_LEN - noPadding) * sizeof(unsigned char));
    memcpy(*out, paddedOut, (noBlocks * BLOCK_LEN - noPadding) * sizeof(unsigned char));

    // free padded output
    free(paddedOut);

    return noBlocks * BLOCK_LEN - noPadding;
}

/*
    KEY SCHEDULING
*/

void aes_generateKeySchedule(unsigned char *in_key, int keylen, unsigned char subkeys[][BLOCK_SIDE][BLOCK_SIDE])
{
    aes_generateKeySchedule128(in_key, subkeys);
}

void aes_generateKeySchedule128(unsigned char *in_key, unsigned char subkeys[11][BLOCK_SIDE][BLOCK_SIDE])
{
    // round 0 takes in the original key
    int i = 0;
    for (int c = 0; c < BLOCK_SIDE; c++)
    {
        for (int r = 0; r < BLOCK_SIDE; r++)
        {
            subkeys[0][r][c] = in_key[i++];
        }
    }

    // generate each round
    unsigned char roundCoeff = 0x01;
    /*
        Round 1: x^0 = 1
        Round 2: x^1 = x
        Round 3: x^2
        ...
        Round 9: x^8 def= x^8 mod P(x)
    */
    for (i = 1; i <= 10; i++)
    {
        // transform key
        unsigned char g[4] = {
            aes_s_box[subkeys[i - 1][1][3]] ^ roundCoeff,
            aes_s_box[subkeys[i - 1][2][3]],
            aes_s_box[subkeys[i - 1][3][3]],
            aes_s_box[subkeys[i - 1][0][3]]};

        for (int r = 0; r < BLOCK_SIDE; r++)
        {
            subkeys[i][r][0] = subkeys[i - 1][r][0] ^ g[r];
        }

        for (int c = 1; c < BLOCK_SIDE; c++)
        {
            for (int r = 0; r < BLOCK_SIDE; r++)
            {
                subkeys[i][r][c] = subkeys[i - 1][r][c] ^ subkeys[i][r][c - 1];
            }
        }

        // increment round coefficient
        roundCoeff = galoisMul(roundCoeff, 0x02); // multiply by x
    }
}