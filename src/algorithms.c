#include <string.h>
#include "algorithms.h"

void insertion_sort_books_by_code(Book *a, size_t n) {
    size_t i;
    for (i = 1; i < n; ++i) {
        Book key = a[i];
        size_t j = i;
        while (j > 0 && a[j - 1].code > key.code) {
            a[j] = a[j - 1];
            --j;
        }
        a[j] = key;
    }
}

void selection_sort_books_by_title(Book *a, size_t n) {
    size_t i, j;
    for (i = 0; i + 1 < n; ++i) {
        size_t menor = i;
        for (j = i + 1; j < n; ++j) {
            if (strcmp(a[j].title, a[menor].title) < 0)
                menor = j;
        }
        if (menor != i) {
            Book tmp = a[i];
            a[i] = a[menor];
            a[menor] = tmp;
        }
    }
}

int sequential_search_book_by_code(const Book *a, size_t n, int code) {
    size_t i;
    for (i = 0; i < n; ++i)
        if (a[i].code == code) return (int)i;
    return -1;
}

int sequential_search_book_by_title(const Book *a, size_t n, const char *title) {
    size_t i;
    for (i = 0; i < n; ++i)
        if (strcmp(a[i].title, title) == 0) return (int)i;
    return -1;
}

int binary_search_book_by_code(const Book *a, size_t n, int code) {
    size_t low = 0, high = n;
    while (low < high) {
        size_t mid = low + (high - low) / 2;
        if (a[mid].code < code)
            low = mid + 1;
        else if (a[mid].code > code)
            high = mid;
        else
            return (int)mid;
    }
    return -1;
}
