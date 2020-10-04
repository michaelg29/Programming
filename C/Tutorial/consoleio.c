#include <stdio.h>

int main()
{
    int c;

    // getchar() reads next character from screen, returns as int
    printf("Enter a value: ");
    c = getchar();

    // putchar(int c) prints character to screen
    printf("\nYou entered: ");
    putchar(c);
    printf("\n");

    char str[100];

    // fflush(buffer) clears buffer of stream
    // call to reset input
    fflush(stdin);

    // gets(char* s) reads line from stdin to s
    printf("Name: ");
    gets(str);

    // puts(char* s) writes string and new line
    printf("Hello, ");
    puts(str);

    // scanf(const char *format) reads input from stdin
    int i;
    scanf("%s %d", str, &i);
    printf("\nYou entered %s %d", str, i);
}