#include "vec.h"
#include <stdio.h>

int main()
{
    vec v1 = newVector(2.0f, -3.0f, 1.0f);
    print(v1);

    vec v2 = newVector(4.0f, -1.0f, 5.0f);
    print(v2);

    vec v3 = cross(v1, v2);
    print(v3);

    return 0;
}