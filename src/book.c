#include <string.h>
#include "book.h"
#include "util.h"

void book_init(Book *b, int id, const char *title, const char *author,
               int year, int quantity) {
    memset(b, 0, sizeof *b);       
    b->id = id;
    b->year = year;
    b->quantity = quantity;
    str_copy(b->title, title, BOOK_TITLE_LEN);
    str_copy(b->author, author, BOOK_AUTHOR_LEN);
}

int book_cmp_id(const Book *a, const Book *b) {
    return (a->id > b->id) - (a->id < b->id);
}

int book_cmp_title(const Book *a, const Book *b) {
    int c = str_icmp(a->title, b->title);
    return c ? c : book_cmp_id(a, b);
}
