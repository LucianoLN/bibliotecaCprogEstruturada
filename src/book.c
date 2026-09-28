#include <stdio.h>
#include <string.h>
#include "../include/book.h"

#define MAX_BOOKS 512

static Book books[MAX_BOOKS];
static int book_count = 0;

void book_init(void) {
    book_count = 0;
}

int book_add(const Book *b) {
    if (!b) return -1;
    if (book_count >= MAX_BOOKS) return -2;
    /* prevent duplicate id */
    for (int i = 0; i < book_count; ++i) {
        if (books[i].id == b->id) return -3;
    }
    books[book_count] = *b; /* struct copy */
    book_count++;
    return 0;
}

Book *book_find_by_id(int id) {
    for (int i = 0; i < book_count; ++i) {
        if (books[i].id == id) return &books[i];
    }
    return NULL;
}

Book *book_find_by_title(const char *title) {
    if (!title) return NULL;
    for (int i = 0; i < book_count; ++i) {
        if (strstr(books[i].title, title) != NULL) return &books[i];
    }
    return NULL;
}

void book_list(void) {
    if (book_count == 0) {
        puts("Nenhum livro cadastrado.");
        return;
    }
    puts("Livros cadastrados:");
    for (int i = 0; i < book_count; ++i) {
        printf("%d) id=%d titulo=%s autor=%s ano=%d qtd=%d\n",
               i + 1,
               books[i].id,
               books[i].title,
               books[i].author,
               books[i].year,
               books[i].quantity);
    }
}

int book_adjust_quantity(int id, int delta) {
    Book *b = book_find_by_id(id);
    if (!b) return -1;
    if (delta < 0 && b->quantity + delta < 0) return -2; /* insufficient */
    b->quantity += delta;
    return 0;
}
