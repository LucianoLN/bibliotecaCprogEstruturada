#ifndef SORT_H
#define SORT_H

#include <stddef.h>
#include "book.h"

typedef int (*BookCmp)(const Book *a, const Book *b);

void sort_insertion(Book *v, size_t n, BookCmp cmp);

void sort_selection(Book *v, size_t n, BookCmp cmp);

int sort_is_sorted(const Book *v, size_t n, BookCmp cmp);

#endif 
