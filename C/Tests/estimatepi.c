#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double rand0to1()
{
    return (double)rand() / (double)RAND_MAX;
}

int main(int argc, char **argv)
{
    srand(time(0));

    int N = 10000;
    printf("%d\n", argc);
    if (argc > 1)
    {
        N = atoi(argv[1]);
    }

    unsigned int numCircle = 0;
    unsigned int numSquare = 0;

    for (int i = 0; i < N; i++)
    {
        double x = rand0to1();
        double y = rand0to1();

        numSquare++;
        if (x * x + y * y < 1)
        {
            numCircle++;
        }
    }

    // r = 1
    // A_square = (2r)^2 = 4
    // A_circle = pi * r^2 = pi

    /*
        A_circle / A_square = pi / 4 ~= numCircle / numSquare
        => pi ~= 4 * numCircle / numSquare
    */

    double pi = 4.0 * (double)numCircle / (double)numSquare;
    printf("Estimated from %d samples:\n", N);
    printf("%d in the circle, %d in the square\n", numCircle, numSquare);
    printf("pi ~= %f\n", pi);

    return 0;
}