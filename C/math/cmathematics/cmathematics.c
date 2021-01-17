#include "cmathematics.h"
#include <stdio.h>

/**
 * method to determine if an integer exists in an array
 * @param arr the array
 * @param noElements the number of elements in the array
 * @param target the target element
 * @return if the element exists in the array
 */
bool containsUint(unsigned int *arr, unsigned int noElements, unsigned int target)
{
    for (unsigned int i = 0; i < noElements; i++)
    {
        if (arr[i] == target)
        {
            return true;
        }
    }

    return false;
}

/**
 * method to print out an integer array
 * @param arr the array
 * @param noElements the number of elements in the array
 */
void printUintArray(unsigned int *arr, unsigned int noElements)
{
    printf("[");
    for (unsigned int i = 0; i < noElements; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("]\n");
}

void divMod(unsigned int num, unsigned int divisor, unsigned int *quotient, unsigned int *remainder)
{
    *quotient = 0;

    while (num >= divisor)
    {
        // subtract one multiple
        num -= divisor;

        // add one to the quotient
        (*quotient)++;
    }

    // the rest is the remainder
    *remainder = num;
}

unsigned int numDigits(unsigned int val, unsigned int base)
{
    unsigned int ret = 0;

    while (val)
    {
        ret++;
        val /= base;
    }

    return ret;
}