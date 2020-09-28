#include <stdio.h>

int main()
{
    // integer literals
    printf("%d\n", 85);   // print decimal
    printf("%d\n", 010);  // print octal number in decimal format
    printf("%o\n", 0213); // print octal number in octal format
    printf("%o\n", 8);    // print number in octal format
    printf("%d\n", 0x4b); // print hexadecimal
    printf("%d\n", 30u);  // print unsigned int
    printf("%d\n", 30l);  // print long
    printf("%d\n", 30ul); // unsigned long

    // floating point literals
    float f = 3.14159;
    f = 3.14159e-5L;

    // characters
    printf("%c\n", 0x02);
    printf("%c\n", '\u20AC'); // universal characters
    // escape characters: \t \n \"

    // define constants with #define
#define LENGTH 10
#define WIDTH 5
#define NEWLINE '\n'

    int area = LENGTH * WIDTH;
    printf("Area: %d%c", area, NEWLINE);

    // define constants with const
    const int LEN = 10;
    const int WID = 5;
    const char NEWL = '\n';

    area = LEN * WIDTH;
    printf("Area: %d%c", area, NEWL);

    return 0;
}