/*
 * search.h - Buscas no acervo. Devolvem o indice no vetor ou -1.
 *
 * [PARTE 1]                         busca sequencial.
 * [COMMIT 9 - BUSCA BINARIA]      busca binaria por codigo e por titulo.
 */
#ifndef SEARCH_H
#define SEARCH_H

#include <stddef.h>
#include "book.h"

/* [PARTE 1] O(n): percorre o vetor, nao exige ordenacao. */
int search_seq_id(const Book *v, size_t n, int id);
/* [PARTE 1] O(n): titulo exato, sem diferenciar maiusculas/minusculas. */
int search_seq_title(const Book *v, size_t n, const char *title);

/* [COMMIT 9] O(log n): EXIGEM o vetor ordenado pelo mesmo campo
 * (book_cmp_id / book_cmp_title); confira com sort_is_sorted antes. */
int search_bin_id(const Book *v, size_t n, int id);
int search_bin_title(const Book *v, size_t n, const char *title);

#endif /* SEARCH_H */
