#ifndef SEARCH_H
#define SEARCH_H

#include <stddef.h>
#include "book.h"

int search_seq_id(const Book *v, size_t n, int id);
int search_seq_title(const Book *v, size_t n, const char *title);

int search_bin_id(const Book *v, size_t n, int id);
int search_bin_title(const Book *v, size_t n, const char *title);

#endif 
