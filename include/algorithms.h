// [COMMIT 8 - ORDENACAO] [COMMIT 9 - BUSCA BINARIA]
#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <stddef.h>
#include "book.h"

void insertion_sort_books_by_code(Book *a, size_t n);
void selection_sort_books_by_title(Book *a, size_t n);

/* Busca binaria exige o vetor ordenado por codigo. Retorna o indice ou -1. */
int binary_search_book_by_code(const Book *a, size_t n, int code);

/* Busca sequencial exigida pela Parte 1 e usada no benchmark. */
int sequential_search_book_by_code(const Book *a, size_t n, int code);
int sequential_search_book_by_title(const Book *a, size_t n, const char *title);

#endif
