#include <stdio.h>
#include <string.h>

/*
    STRUCTURES
    - hold several data items of different types
    - combine different data
    - represent records
*/

struct Book
{
    char title[50];
    char author[50];
    char subject[100];
    int book_id;
};

typedef struct Book Book;

void printBookInfo(Book book);

int main()
{
    Book b1;

    // set data using strcpy
    strcpy(b1.title, "Book Title1");
    strcpy(b1.author, "Book Author1");
    strcpy(b1.subject, "Subject 1");
    b1.book_id = 5912;

    // pass as argument
    printBookInfo(b1);

    // pointer
    Book *ptr = &b1;
    ptr->book_id++;
}

void printBookInfo(Book book)
{
    /* print Book1 info */
    printf("Book title : %s\n", book.title);
    printf("Book author : %s\n", book.author);
    printf("Book subject : %s\n", book.subject);
    printf("Book book_id : %d\n", book.book_id);
}