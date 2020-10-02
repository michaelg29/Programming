#include <stdio.h>

/*
    Bit Fields
    - packing of data in structure
        - can have 1 bit flags
        - read non-standard file formats (9 bit ints)

    struct {
        type member_name : width;
    };

    size rounds up to nearest word
*/

// size = 8
// 4 bytes + 4 bits + 5 bits = 5 bytes + 1 bit -> round up to nearest word = 8 bytes
typedef struct
{
    unsigned int d : 5; // 1 - 31, 5 bits fine
    unsigned int m : 4; // 1 - 12, 4 bits fine
    unsigned int y;
} date;

typedef struct
{
    int d : 6; // can have negative values
    int m : 5;
    int y;
} weirdDate;

/*
    force alignment
*/

// A structure without forced alignment
// size = 4 bytes = 1 word
typedef struct
{
    unsigned int x : 5;
    unsigned int y : 8;
} test1;

// A structure with forced alignment
// size = 8 bytes = 2 words
typedef struct
{
    unsigned int x : 5;
    unsigned int : 0; // go to next word
    unsigned int y : 8;
} test2;

int main()
{
    printf("Size of date is %lu bytes\n", sizeof(date));

    date dt = {31, 12, 2014};
    printf("Date is %d-%d-%d\n", dt.d, dt.m, dt.y); // 31-12-2014

    weirdDate wd = {31, 12, 2014};
    printf("Date is %d-%d-%d\n", wd.d, wd.m, wd.y); // -1/-4/2014

    /*
        two's complement

        with 31, bin(31) = 0b11111
            MSB = 1 -> will be considered negative if only 5 bits
            if allocate 6 bits, MSB = 0 so considered position

        with 12, bin(12) = 0b1100
            MSB = 1 -> considered negative if only 4 bits
            if allocate 5 bits, MSB = 0 so considered positive
    */

    printf("Test 1 size: %lu\n", sizeof(test1));
    printf("Test 2 size: %lu\n", sizeof(test2));

    // =========================
    // cannot have pointers because may not start at byte boundary

    test1 t1;
    // causes error:
    //printf("Address of t1.x is %p\n", &t1.x);

    // can do this because starts at next word
    printf("Address of dt.y is %p\n", &dt.y);

    // ===========================
    // assigning out of range values
    //dt.d = 50; // causes overflow error

    // ==========================
    // cannot have arrays in bitfields

    return 0;
}