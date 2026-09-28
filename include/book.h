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

/* Book management API (in-memory) */
void book_init(void);
int book_add(const Book *b); /* returns 0 on success, <0 on error */
Book *book_find_by_id(int id);
Book *book_find_by_title(const char *title);
void book_list(void);
int book_adjust_quantity(int id, int delta); /* delta can be negative */


#endif