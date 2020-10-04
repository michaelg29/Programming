#include <stdio.h>
#include <errno.h>
#include <string.h>

extern int errno; // defined in errno.h

int main()
{
    FILE *pf;
    int errnum = 0;

    pf = fopen("file.txt", "rb");

    if (!pf)
    {
        errnum = errno;
        fprintf(stderr, "Value of errno: %d\n", errno);
        perror("Error printed by perror");
        fprintf(stderr, "Error opening file: %s\n", strerror(errnum));
    }
    else
    {
        fclose(pf);
    }

    return errnum;
}