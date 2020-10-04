#include <stdio.h>
#include <stdarg.h>

double avg(int num, ...)
{
    // get list of arguments
    va_list list;
    va_start(list, num);

    double sum = 0.0;

    // number of arguments must be passed in before
    for (int i = 0; i < num; i++)
    {
        // access next argument of type
        sum += va_arg(list, int);
    }

    // clear memory
    va_end(list);

    return sum / num;
}

int main()
{
    printf("avg = %f\n", avg(2, 3, 5, 1, -2, 5));
}