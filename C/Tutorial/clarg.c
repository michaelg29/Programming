#include <stdio.h>

int main(int argc, char *argv[])
{
    // argc includes run command (ie a.exe)
    if (argc == 2)
    {
        printf("The argument supplied is %s\n", argv[1]);
        return 0;
    }
    else if (argc > 2)
    {
        printf("Too many arguments supplied.\n");
        return 1;
    }
    else
    {
        printf("One argument expected.\n");
        return 1;
    }
}