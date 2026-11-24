/*
 * sort.c - Algoritmos de ordenacao escritos a mao (sem qsort).
 *
 * [COMMIT 8 - ORDENACAO]          Insertion Sort e Selection Sort.
 */
#include "sort.h"

void sort_insertion(Book *v, size_t n, BookCmp cmp) {
    size_t i, j;
    for (i = 1; i < n; i++) {
        Book key = v[i];            /* livro a inserir na parte ja ordenada v[0..i-1] */
        j = i;
        while (j > 0 && cmp(&v[j - 1], &key) > 0) {
            v[j] = v[j - 1];        /* empurra os maiores uma posicao para a direita */
            j--;
        }
        v[j] = key;
    }
}

void sort_selection(Book *v, size_t n, BookCmp cmp) {
    size_t i, j, min;
    for (i = 0; i + 1 < n; i++) {
        min = i;                    /* procura o menor em v[i..n-1] */
        for (j = i + 1; j < n; j++)
            if (cmp(&v[j], &v[min]) < 0) min = j;
        if (min != i) {
            Book tmp = v[i];
            v[i] = v[min];
            v[min] = tmp;
        }
    }
}

int sort_is_sorted(const Book *v, size_t n, BookCmp cmp) {
    size_t i;
    for (i = 1; i < n; i++)
        if (cmp(&v[i - 1], &v[i]) > 0) return 0;
    return 1;
}
