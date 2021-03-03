#include <stdarg.h>

#include "cmathematics.h"

#ifndef BIGINT_H
#define BIGINT_H

#define BASE 1000000000       // base used to represent each digit in the big integer
#define NO_BASE_DIGITS 9      // number of base 10 digits the base uses
#define KARATSUBA_THRESHOLD 2 // threshold between performing long multiplication and karatsuba multiplication

/**
 * structure representing an integer
 * each integer in the array corresponds to one digit in base 10^9
 */
typedef struct
{
    bool sign;             // the sign of the number (true if 0 or positive, false if negative)
    unsigned int capacity; // number of slots in the array
    unsigned int noDigits; // number of used slots in the array
    int *digits;           // array of the digits in reverse order; digits[i] = BASE^i component
} bigint;

extern const bigint BIGINT_ZERO;    // zero integer
extern const bigint BIGINT_ONE;     // one
extern const bigint BIGINT_NEG_ONE; // negative one

/**
 * allocate memory for the integer
 * @param capacity the initial size of the array
 * @return the integer
 */
bigint allocateBigint(unsigned int capacity);

/**
 * allocate memory for the integer and set all the digits to 0
 * @param capacity the initial size of the array
 * @return the integer
 */
bigint allocateZeroBigint(unsigned int capacity);

/**
 * copy integer array to a big integer
 * @param arr the pointer to the first integer in the array
 * @param n the number of digits
 * @param sign the sign
 */
bigint copyIntArr(int *arr, unsigned int n, bool sign);

/**
 * free the memory of a big integer
 * @param i the integer
 */
void freeBigint(bigint *i);

/**
 * convert string to a big integer by parsing each digit
 * 
 * PROCESS
 * for each block of 9 base 10 digits
 * val = digit[8] * 10^8 + digit[7] * 10^7 + ... + digit[0] + 10^0
 *     = ((digit[8] * 10 + digit[7]) * 10 + ... ) * 10 + digit[0]
 * 
 * @param str the string
 * @return the integer
 */
bigint strToBigint(char *str);

/**
 * creates new positive big integer from an unsigned integer
 * 
 * PROCESS - conversion of bases
 * i_10 / BASE = i' R digits[0] = LSD (least significant digit)
 * i' / BASE = i'' R digits[1]
 * ...
 * i''' / BASE = 0 R digits[N - 1] = MSD (most significant digit)
 * 
 * @param i the integer
 * @return the integer
 */
bigint newPositiveBigint(unsigned int i);

/**
 * creates new positive big integer from an unsigned long long
 * same process as newPositiveBigInt
 * 
 * @param l the num
 * @return the big integer
 */
bigint newLLPositiveBigInt(unsigned long long l);

/**
 * creates new big integer from an integer
 * calls newPositiveBigInt then applies sign after
 * @param i the integer
 * @return the big integer
 */
bigint newBigint(int i);

/**
 * creates new big integer from a long long
 * calls newLLPositiveBigInt then applies sign after
 * @param i the long long
 * @return the big integer
 */
bigint newLLBigInt(long long l);

/**
 * multiplies two integers together and handles possible overflow
 * @param i1 the first integer
 * @param i2 the second integer
 * @return the big integer containing the product
 */
bigint multiplyIntInt(unsigned int i1, unsigned int i2);

/**
 * trims leading zeros in the array of digits
 * @param b the pointer to the integer to trim
 */
void trimBigint(bigint *b);

/**
 * get string representation of number
 * 
 * EXAMPLE
 * [456789780, 123] => "123456789780"
 * 
 * digitIdx strIdx  char    offset
 * 1        0       '3'     2
 *                  '2'     1
 *                  '1'     0 => "123"
 * 2        3       '0'     8
 *                  '8'     7
 *                  ...
 *                  '5'     1
 *                  '4'     0 => "456789780"
 *                               -----------
 *                               "123456789780"
 * 
 * @param i the pointer to the big integer
 * @return the string
 */
char *bigintPtrToString(bigint *i);

/**
 * get string representation of number
 * @param i the big integer
 * @return the string
 */
char *bigintToString(bigint i);

/**
 * comparison of two integers
 * @param i1 the first integer
 * @param i2 the second integer
 * @return -1 if i1 < i2, 0 if i1 = i2, 1 if i1 > i2
 */
char compareBigint(bigint i1, bigint i2);

/**
 * adds two integers
 * @param i1 the first integer
 * @param i2 the second integer
 * @return the sum
 */
bigint addBigint(bigint i1, bigint i2);

/**
 * subtracts two integers
 * @param i1 the integer to subtract from
 * @param i2 the integer to subtract
 * @return the difference
 */
bigint subtractBigint(bigint i1, bigint i2);

/**
 * multiplies two integers with elementary multiplication
 * @param i1 the first integer
 * @param i2 the second integer
 * @return the product
 */
bigint multiplyBigint(bigint i1, bigint i2);

bigint longMultiplyBigint(bigint i1, bigint i2);

int *longMultiplyIntArr(int *i1, unsigned int i1i, unsigned int i1f, int *i2, unsigned int i2i, unsigned int i2f, unsigned int *outSize);

bigint karatsubaMultiplyBigint(bigint i1, bigint i2);

int *karatsubaMultiplyIntArr(int *i1, unsigned int i1Size, int *i2, unsigned int i2Size, unsigned int idxi, unsigned int idxf, unsigned int *outSize);

#endif