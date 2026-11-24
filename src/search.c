/*
 * search.c - Buscas no acervo.
 *
 * [PARTE 1]                         busca sequencial.
 * [COMMIT 9 - BUSCA BINARIA]      busca binaria por codigo e por titulo.
 */
#include "search.h"
#include "util.h"

int search_seq_id(const Book *v, size_t n, int id) {
    size_t i;
    for (i = 0; i < n; i++)
        if (v[i].id == id) return (int)i;
    return -1;
}

int search_seq_title(const Book *v, size_t n, const char *title) {
    size_t i;
    for (i = 0; i < n; i++)
        if (str_icmp(v[i].title, title) == 0) return (int)i;
    return -1;
}

/* [COMMIT 9] A cada passo descarta a metade do intervalo [lo, hi) que nao
 * pode conter a chave. mid = lo + (hi - lo) / 2 evita overflow de lo + hi. */
int search_bin_id(const Book *v, size_t n, int id) {
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (v[mid].id == id) return (int)mid;
        if (v[mid].id < id) lo = mid + 1;
        else hi = mid;
    }
    return -1;
}

int search_bin_title(const Book *v, size_t n, const char *title) {
    size_t lo = 0, hi = n;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        int c = str_icmp(v[mid].title, title);
        if (c == 0) return (int)mid;
        if (c < 0) lo = mid + 1;
        else hi = mid;
    }
    return -1;
}
