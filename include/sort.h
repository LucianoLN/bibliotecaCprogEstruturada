/*
 * sort.h - Algoritmos de ordenacao escritos a mao (sem qsort).
 *
 * [COMMIT 8 - ORDENACAO]          Insertion Sort e Selection Sort, com o
 *                                   criterio passado como ponteiro de funcao.
 */
#ifndef SORT_H
#define SORT_H

#include <stddef.h>
#include "book.h"

/* Criterio de ordem: <0 se a vem antes de b, 0 se empatam, >0 se depois. */
typedef int (*BookCmp)(const Book *a, const Book *b);

/* Insertion Sort - O(n^2) no pior caso, O(n) se ja estiver quase ordenado.
 * Estavel: livros empatados mantem a ordem original. */
void sort_insertion(Book *v, size_t n, BookCmp cmp);

/* Selection Sort - sempre O(n^2) comparacoes, mas no maximo n-1 trocas. */
void sort_selection(Book *v, size_t n, BookCmp cmp);

/* 1 se o vetor ja esta em ordem segundo cmp (usado antes da busca binaria). */
int sort_is_sorted(const Book *v, size_t n, BookCmp cmp);

#endif /* SORT_H */
