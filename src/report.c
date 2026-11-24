#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "report.h"
#include "search.h"
#include "sort.h"
#include "util.h"

static const size_t SIZES[] = {1000, 2000, 5000, 10000};  
#define N_SIZES (sizeof SIZES / sizeof SIZES[0])
#define QUERIES 1000      
#define MIN_MS 100.0         
#define SEED 12345u

typedef void (*SortFn)(Book *, size_t, BookCmp);
typedef int (*SearchFn)(const Book *, size_t, int);

typedef struct Row {
    size_t n;
    double sort_ms[2][2];      
    double seq_us, bin_us;   
} Row;

static double ms_since(clock_t start) {
    return (double)(clock() - start) * 1000.0 / CLOCKS_PER_SEC;
}

static size_t rand_index(size_t n) {
    return (((size_t)rand() << 15) ^ (size_t)rand()) % n;
}

static void fill_random(Book *v, size_t n) {
    size_t i;
    char title[11];
    for (i = 0; i < n; i++) {
        int k;
        for (k = 0; k < 10; k++) title[k] = (char)('a' + rand() % 26);
        title[10] = '\0';
        book_init(&v[i], (int)i + 1, title, "Autor", 2000, 1);
    }
    for (i = n - 1; i > 0; i--) {
        size_t j = rand_index(i + 1);
        int tmp = v[i].id;
        v[i].id = v[j].id;
        v[j].id = tmp;
    }
}

static double time_sort(SortFn sort, BookCmp cmp, const Book *base, Book *work, size_t n) {
    clock_t start;
    double ms;
    memcpy(work, base, n * sizeof *work);
    start = clock();
    sort(work, n, cmp);
    ms = ms_since(start);
    if (!sort_is_sorted(work, n, cmp)) fprintf(stderr, "ERRO: vetor nao ficou ordenado!\n");
    return ms;
}

static double time_search(SearchFn search, const Book *v, size_t n, const int *keys) {
    long rounds = 0, found = 0;
    double ms;
    clock_t start = clock();
    do {
        int q;
        for (q = 0; q < QUERIES; q++) found += search(v, n, keys[q]) >= 0;
        rounds++;
    } while ((ms = ms_since(start)) < MIN_MS);
    if (found != rounds * QUERIES) fprintf(stderr, "ERRO: busca nao achou um codigo existente!\n");
    return ms * 1000.0 / ((double)rounds * QUERIES);
}

static int measure(size_t n, Row *row) {
    static const SortFn algos[2] = {sort_insertion, sort_selection};
    static const BookCmp cmps[2] = {book_cmp_id, book_cmp_title};
    Book *base = malloc(n * sizeof *base);
    Book *work = malloc(n * sizeof *work);
    int *keys = malloc(QUERIES * sizeof *keys);
    int a, c, q, ok = base && work && keys;

    if (ok) {
        fill_random(base, n);
        row->n = n;
        for (a = 0; a < 2; a++)
            for (c = 0; c < 2; c++)
                row->sort_ms[a][c] = time_sort(algos[a], cmps[c], base, work, n);
        for (q = 0; q < QUERIES; q++) keys[q] = (int)rand_index(n) + 1;
        row->seq_us = time_search(search_seq_id, base, n, keys);
        memcpy(work, base, n * sizeof *work);
        sort_insertion(work, n, book_cmp_id);
        row->bin_us = time_search(search_bin_id, work, n, keys);
    }
    free(base);
    free(work);
    free(keys);
    return ok ? 0 : -1;
}

static double ratio(double a, double b) {
    return b > 0 ? a / b : 0;
}

static void print_report(FILE *out, const Row *rows, size_t n_rows) {
    static const char *algo_name[2] = {"Insertion", "Selection"};
    static const char *field_name[2] = {"codigo", "titulo"};
    const Row *last = &rows[n_rows - 1], *prev = &rows[n_rows - 2];
    char today[16];
    size_t i;
    int c;

    date_format(date_today(), today, sizeof today);
    fprintf(out, "RELATORIO DE DESEMPENHO - SGB (%s)\n", today);
    fprintf(out, "Dados sinteticos: codigos embaralhados e titulos aleatorios de 10 letras.\n");
    fprintf(out, "Tempo medido com clock() de <time.h> (CLOCKS_PER_SEC = %ld).\n\n",
            (long)CLOCKS_PER_SEC);

    fprintf(out, "1) ORDENACAO - tempo total (ms)\n");
    fprintf(out, "%7s | %14s %14s | %14s %14s\n", "N",
            "Insertion/cod", "Selection/cod", "Insertion/tit", "Selection/tit");
    for (i = 0; i < n_rows; i++)
        fprintf(out, "%7lu | %14.2f %14.2f | %14.2f %14.2f\n", (unsigned long)rows[i].n,
                rows[i].sort_ms[0][0], rows[i].sort_ms[1][0],
                rows[i].sort_ms[0][1], rows[i].sort_ms[1][1]);

    fprintf(out, "\n2) BUSCA POR CODIGO - tempo medio por busca (us), %d chaves aleatorias\n",
            QUERIES);
    fprintf(out, "%7s | %12s %12s | %14s\n", "N", "Sequencial", "Binaria", "Ganho binaria");
    for (i = 0; i < n_rows; i++)
        fprintf(out, "%7lu | %12.3f %12.3f | %13.0fx\n", (unsigned long)rows[i].n,
                rows[i].seq_us, rows[i].bin_us, ratio(rows[i].seq_us, rows[i].bin_us));

    fprintf(out, "\n3) ANALISE (calculada a partir das medicoes acima)\n");
    for (c = 0; c < 2; c++) {
        int fast = last->sort_ms[0][c] <= last->sort_ms[1][c] ? 0 : 1;
        fprintf(out, "- Por %s com N = %lu: %s foi %.1fx mais rapido que %s.\n",
                field_name[c], (unsigned long)last->n, algo_name[fast],
                ratio(last->sort_ms[1 - fast][c], last->sort_ms[fast][c]), algo_name[1 - fast]);
    }
    fprintf(out, "- De N = %lu para N = %lu (%.0fx mais dados) o Selection Sort por codigo\n"
                 "  ficou %.1fx mais lento: crescimento quadratico, O(n^2).\n",
            (unsigned long)prev->n, (unsigned long)last->n, ratio((double)last->n, (double)prev->n),
            ratio(last->sort_ms[1][0], prev->sort_ms[1][0]));
    fprintf(out, "- Insertion Sort desloca structs inteiras (%lu bytes cada); Selection Sort\n"
                 "  compara mais, porem troca no maximo n-1 vezes.\n", (unsigned long)sizeof(Book));
    fprintf(out, "- Busca: a sequencial e O(n) e cresce junto com N; a binaria e O(log n) e\n"
                 "  quase nao muda. Com N = %lu a binaria foi %.0fx mais rapida, mas so\n"
                 "  funciona com o vetor ordenado: a ordenacao (O(n^2) aqui) e paga uma vez\n"
                 "  e compensa quando ha muitas buscas sobre o mesmo acervo.\n",
            (unsigned long)last->n, ratio(last->seq_us, last->bin_us));
}

int report_run(const char *path) {
    Row rows[N_SIZES];
    size_t i;
    FILE *f;

    srand(SEED);
    for (i = 0; i < N_SIZES; i++) {
        printf("Medindo N = %lu...\n", (unsigned long)SIZES[i]);
        fflush(stdout);
        if (measure(SIZES[i], &rows[i]) != 0) return -1;
    }
    printf("\n");
    print_report(stdout, rows, N_SIZES);

    f = fopen(path, "w");
    if (!f) return -1;
    print_report(f, rows, N_SIZES);
    return fclose(f) == 0 ? 0 : -1;
}
