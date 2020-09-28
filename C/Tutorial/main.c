#include <stdio.h>

// global variable declaration
int global_a, global_b;
float global_f;

void print();

int main()
{
    printf("Hello, world!\n");

    // variable definition
    int a, b;
    float f;

    // variable initialization
    a = 10;
    b = 20;
    f = 80.0f;

    global_a = 10;
    global_b = 20;
    global_f = 80.0f;

    printf("%d %d %f\n", a, b, f);

    print();

    return 0;
}

void print()
{
    printf("%d %d %f\n", global_a + 5, global_b + 6, global_f * 2.0f);
}