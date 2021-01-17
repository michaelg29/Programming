#include <stdio.h>

#include "../cmathematics/cmathematics.h"
#include "../cmathematics/vec.h"
#include "../cmathematics/matrix.h"
#include "../cmathematics/bigint.h"

int main()
{
    printf("Hello, world!\n");

    bigint i = strToBigint("123456789780");
    bigint i2 = strToBigint("123456789780");

    printf("%s + %s = %s\n",
           bigintPtrToString(&i),
           bigintPtrToString(&i2),
           bigintToString(addBigint(i, i2)));

    return 0;
}