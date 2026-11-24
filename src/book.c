/*
 * book.c - Funcoes do modelo Book.
 *
 * [PARTE 1]                         book_init.
 * [COMMIT 6 - ALOCACAO DINAMICA]  inicializa quantity.
 * [COMMIT 8 - ORDENACAO]          comparadores por codigo e por titulo.
 */
#include <string.h>
#include "book.h"
#include "util.h"

void book_init(Book *b, int id, const char *title, const char *author,
               int year, int quantity) {
    memset(b, 0, sizeof *b);        /* sem lixo de memoria nos arquivos binarios */
    b->id = id;
    b->year = year;
    b->quantity = quantity;
    str_copy(b->title, title, BOOK_TITLE_LEN);
    str_copy(b->author, author, BOOK_AUTHOR_LEN);
}

/* [COMMIT 8] (x > y) - (x < y) da -1, 0 ou 1 sem risco de overflow. */
int book_cmp_id(const Book *a, const Book *b) {
    return (a->id > b->id) - (a->id < b->id);
}

int book_cmp_title(const Book *a, const Book *b) {
    int c = str_icmp(a->title, b->title);
    return c ? c : book_cmp_id(a, b);
}
