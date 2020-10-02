#include <stdio.h>
#include <string.h>

int main()
{
    /*
        0   1   2   3   4   5
        H   e   l   l   o   \0
    */
    char greeting[] = "Hello";
    char greeting2[6] = {'H', 'e', 'l', 'l', 'o', '\0'};
    printf("Greeting: %s\n", greeting);
    printf("Greeting: %s\n", greeting2);

    /*
        functions
    */

    char str1[12] = "Hello";
    char str2[12] = "World";
    char str3[12];
    int len;

    // copy string s2 into s1
    // strcpy(s1, s2);
    strcpy(str3, str1);
    printf("%s | %s | %s\n", str1, str2, str3);

    // concatenate s2 onto s1
    // strcat(s1, s2);
    strcat(str1, str2);
    printf("%s | %s | %s\n", str1, str2, str3);

    // string length
    // strlen(s1);
    len = strlen(str1);
    printf("Len: %d\n", len);

    // compare strings
    // strcmp(s1, s2);
    // return ?0 if s1 ? s2 (? can be =, <, >)
    printf("%d\n", strcmp(str1, str2));

    // get pointer to first occurence of char c in s1
    // strchr(s1, ch);
    printf("%d\n", (strchr(str1, 'l') - &str1[0])); // gets first index

    // get pointer to first occurence of string s2 in s1
    // strstr(s1, s2);
    printf("%d\n", (strstr(str1, "ll") - &str1[0]));

    return 0;
}