#include <iostream>
#include "storage_classes.h"

/*
    compile with
    g++ storage_classes_main.c storage_classes.h storage_classes.c -o main    
*/
int main()
{
    callCount = 5;
    changeCount();
    changeCount();
    changeCount();
    printf("Call count = %d\n", callCount);
}