#include <stdio.h>
#include <cstdlib>

/*
    pass to functions
*/

// return name(type* array) {}
double avg1(int *array, int size);

// return name(type array[size]); {}
double avg2(int array[10]);

// return name(type array[]) {}
// signature equivalent to avg1
double avg3(int array[], int size);

/*
    return from functions
*/
int *getRandom(int n);

int main()
{
    // type name[size];
    double balance[10];

    // type name[size] = { default value };
    double balance2[10] = {1.0};

    // type name[size] = { values };
    double balance3[4] = {1000.0, 2.0, 3.0, 4.0};

    // type name[] = { values };
    double balance4[] = {199.0, 23.42, 1.2, 5.3};

    // type* name = (type*)malloc(size * sizeof(type));
    double *balance5 = (double *)malloc(5 * sizeof(double));

    // set values
    for (int i = 0; i < 10; i++)
    {
        balance[i] = 3 * i + 50;
    }

    // get values
    for (int i = 0; i < 10; i++)
    {
        printf("Element[%d] = %f\n", i, balance[i]);
    }

    /*
        multi-dimensional arrays
    */
    // type name[size1][size2]...[sizeN];
    int array[3][4];

    // type name[size1][size2]...[sizeN] = { { { ...}}};
    int array2[3][4] = {
        {0, 1, 2, 3},  /*  initializers for row indexed by 0 */
        {4, 5, 6, 7},  /*  initializers for row indexed by 1 */
        {8, 9, 10, 11} /*  initializers for row indexed by 2 */
    };

    // type name[size1][size2]...[sizeN] = { values };
    int array3[3][4] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    for (int r = 0; r < 3; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            printf("[%d][%d] = %d\n", r, c, array2[r][c]);
        }
    }

    // pass as argument
    int test[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    printf("%f\n", avg1(&test[0], 10));
    printf("%f\n", avg2(test));
    printf("%f\n", avg3(test, 10));

    // get as return
    int *randArray = getRandom(5);
    for (int i = 0; i < 5; i++)
    {
        printf("*(p + %d) : %d = %d\n", i, *(randArray + i), randArray[i]);
    }

    /*
        pointer to array
    */
    // randArray = &randArray[0]
    // get value with *(randArray + i) or randArray[i]
}

// return name(type* array) {}
double avg1(int *array, int size)
{
    double sum = 0.0;
    for (int i = 0; i < size; i++)
    {
        sum += array[i];
    }
    return sum / size;
}

// return name(type array[size]); {}
double avg2(int array[10])
{
    double sum = 0.0;
    for (int i = 0; i < 10; i++)
    {
        sum += array[i];
    }
    return sum / 10;
}

// return name(type array[]) {}
double avg3(int array[], int size)
{
    double sum = 0.0;
    for (int i = 0; i < size; i++)
    {
        sum += array[i];
    }
    return sum / size;
}

int *getRandom(int n)
{
    // declare static so stays alive after function
    static int r[10];

    // set random seed
    //srand((unsigned)time(NULL));

    for (int i = 0; i < 10; i++)
    {
        r[i] = rand();
        printf("r[%d] = %d\n", i, r[i]);
    }

    return r;
}