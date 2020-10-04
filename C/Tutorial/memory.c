#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    /*
        ========================
        allocating and deallocating memory
    */

    char *description;
    char *author;

    // malloc(noBytes) returns pointer to memory of size noBytes
    description = malloc(200 * sizeof(char)); // equivalent to: char description[200];

    if (!description)
    {
        printf("could not allocate memory\n");
    }
    else
    {
        strcpy(description, "This is the description");
        printf("%s\n", description);
    }

    // realloc(ptr, newsize) changes size of memory associated with variable (can increase or decrease)
    description = realloc(description, 250 * sizeof(char));

    if (!description)
    {
        printf("could not allocate additional memory\n");
    }
    else
    {
        strcat(description, "This is a good thing");
        printf("%s\n", description);
    }

    // free(ptr) deallocates memory associated with variable
    free(description);

    // calloc(num, size) returns pointer to memory of size = num * size
    author = calloc(50, 1);

    if (!author)
    {
        printf("could not allocate memory\n");
    }
    else
    {
        strcpy(author, "name name");
        printf("%s\n", author);
    }

    // free
    free(author);

    return 0;
}