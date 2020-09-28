#include <stdio.h>
#include "storage_classes.h"

/*
    storage classes define scope and lifetime of variables/functions
*/

// global static variable
static int count = 5;

// function declaration
void func(void);

// global extern variable from header
int callCount = 0;

// global extern function
extern void storage()
{
    /*
        auto - default
    */
    auto number = 5;
    {
        auto number = 20;
        printf("inner number: %d\n", number);
    }
    printf("outer number: %d\n", number);

    /*
        register
        - store in register rather than RAM
        - has max size equal to register size
        - cannot have unary '&' operator bc no memory location
    */
    register int miles;

    /*
        static
        - keep alive for entire program (don't create/destroy when goes in/out of scope)
    */
    while (count--)
    {
        func();
    }
}

void changeCount()
{
    /*
        extern
        - global variable visible to all files
        - points to location of memory

        define in storage_classes_main
    */
    callCount++;
}

void func(void)
{
    // local static variable
    static int i = 5;
    i++;

    printf("i is %d and count is %d\n", i, count);
}