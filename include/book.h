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

void book_init(Book *b, int id, const char *title, const char *author,
               int year, int quantity);

int book_cmp_id(const Book *a, const Book *b);
int book_cmp_title(const Book *a, const Book *b);

#endif 
