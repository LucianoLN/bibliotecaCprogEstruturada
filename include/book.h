#ifndef BOOK_H
#define BOOK_H

#define BOOK_TITLE_LEN 128
#define BOOK_AUTHOR_LEN 128

typedef struct Book {
    int id;
    char title[BOOK_TITLE_LEN];
    char author[BOOK_AUTHOR_LEN];
    int year;
    int quantity;
} Book;


#endif