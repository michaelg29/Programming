#include <stdio.h>
#include <string.h>

typedef struct
{
    unsigned char age[2];
    unsigned char name[15];
} Student;

int main()
{
    // declare file pointer
    FILE *fp;

    // fopen(const char *filename, const char *mode) opens file and returns pointer
    /*
        MODES
        r - read
        w - write (creates if doesn't exist)
        a - append (creates if doesn't exist)
        r+ - open for reading and writing
        w+ - open for reading and writing (both cursors start at beginning)
        a+ - open for reading and writing (reading cursor starts at beginning, writing starts at end)

        to handle binary format:
        put b in format
        eg. rb, wb, ab, rb+, r+b, ...
    */
    fp = fopen("test.txt", "w+");

    /*
        ======================
        WRITING
    */

    // fprintf writes to stream like printf
    fprintf(fp, "This is writing from fprintf with %d\n", 5);

    // fputc(int c, FILE *fp) writes individual character to stream
    fputc('c', fp);

    // fputs(const char* s, FILE *fp) writes string to stream
    fputs("Test with fputs\n", fp);

    // fclose(FILE *fp) closes file
    fclose(fp);

    /*
        ======================
        READING
    */
    char buff[255];
    char c;
    fp = fopen("test.txt", "r");

    // fscanf reads from stream like scanf
    fscanf(fp, "%s", buff);
    printf("1: %s\n", buff);

    // fgetc(FILE *fp) gets character)
    c = fgetc(fp);
    printf("2: %c\n", c);

    // fgets(char* buf, int n, FILE *fp)
    fgets(buff, 255, fp);
    printf("3: %s\n", buff);

    // fclose(FILE *fp) closes file
    fclose(fp);

    /*
        =======================
        BINARY IO
        write with arrays
    */

    fp = fopen("students.txt", "a+");

    // fread(void* ptr, size_t sizeOfElements, size_t noElements, FILE *fp);
    Student s1;
    fread(&s1, 1, sizeof(Student), fp);
    printf("%c\n%c\n", s1.age[0], s1.age[1]);
    printf("%s is\n%s years old\n", s1.name, s1.age);
    //fwrite(s1.age, 1, 2, fp);
    //fwrite(s1.name, 1, strlen(s1.name), fp);

    putc('\n', fp);

    // fwrite(const void* ptr, size_t sizeOfElements, size_t noElements, FILE *fp);
    Student s2;
    strcpy(s2.age, "19");
    strcpy(s2.name, "Name2");
    fwrite(s2.age, 1, 2, fp);
    fwrite(s2.name, 1, strlen(s2.name), fp);

    fclose(fp);
}