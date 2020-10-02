#include <stdio.h>
#include <string.h>

/*
    UNIONS
    - store different data types in same memory location
    - allocated size is max possible
*/

// can store integer, float, or string
// size = 20
union Data
{
    int i;
    float f;
    char str[20];
};

typedef union Data Data;

int main()
{
    Data data;
    printf("Size of union: %d\n", sizeof(data));

    data.i = 10;
    data.f = 220.5;
    strcpy(data.str, "Test string");

    // can only use one member at a time
    printf("data.i: %d\n", data.i); // corrupted value
    printf("data.f: %f\n", data.f); // corrupted value
    printf("data.str: %s\n", data.str);

    return 0;
}