#include <stdio.h>
#include <time.h>

void getSeconds(unsigned long *par);

int main()
{
    // type *var_name;
    /*
        POINTERS
        - like Java reference
        - variable whose value is address of another variable
        - direct address of memory location
    */

    int var1;
    printf("Address of var1: %x\n", &var1); // %x is hexadecimal

    // declare variable
    int var = 20;
    // declare pointer and store address of var
    int *ip = &var;
    // get value
    int var2 = *ip + 30; // 50

    printf("Address of var: %x\n", &var);
    printf("Address stored in ip: %x\n", ip);
    printf("Value of memory at ip: %d\n", *ip);

    // NULL pointers
    // don't have exact address
    int *ptr = NULL;
    printf("Address stored in ptr: %x\n", ptr); // 0
    if (ptr)
    {
        // pointer points to something
        printf("ptr points to a value\n");
    }
    else
    {
        // null pointer
        printf("ptr points to nothing\n");
    }

    /*
        pointer arithmetic
        ++, --, +, -
    */

    ptr++; // advances 4 bytes because sizeof(int) = 4

    int arr[] = {10, 100, 200};
    int i, *arrptr;

    /* let us have array address in pointer */
    arrptr = arr;

    for (i = 0; i < 3; i++)
    {
        printf("Address of var[%d] = %x\n", i, arrptr);
        printf("Value of var[%d] = %d\n", i, *arrptr);

        /* move to the next location */
        arrptr++; // can decrement to go in opposite direction
    }

    /*
        pointer comparisons
        ==, <, >, <=, >=
    */

    // compare positions
    i = 0;
    arrptr = arr;

    while (arrptr <= &arr[2])
    {
        // move to next location
        arrptr++;
        i++;
    }

    /*
        array of pointers
    */

    // array of addresses
    int *ptrArr[3];
    for (i = 0; i < 3; i++)
    {
        ptrArr[i] = &arr[i];
        printf("Vals[%d] = %d\n", i, *ptrArr[i]);
    }

    // array of strings
    char *names[] = {
        "Zara Ali",
        "Hina Ali",
        "Nuha Ali",
        "Sara Ali"};

    for (i = 0; i < 3; i++)
    {
        printf("names[%d] = %s\n", i, names[i]);
    }

    /*
        nested pointers
        (ie pointer to a pointer to ...)
    */

    var = 50;
    ptr = &var;
    int **pptr = &ptr;

    /* take the value using pptr */
    printf("Value of var = %d\n", var);
    printf("Value available at *ptr = %d\n", *ptr);
    printf("Value available at **pptr = %d\n", **pptr);

    /*
        passing as argument
        - allow value at address to be modified
        - pass arrays (see arrays.c)
    */
    unsigned long sec;
    getSeconds(&sec);
    printf("Number of seconds: %ld\n", sec);

    /*
        getting as return value
        - return arrays (see arrays.c)
    */

    return 0;
}

void getSeconds(unsigned long *par)
{
    *par = time(NULL);
}