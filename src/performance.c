// [COMMIT 12 - RELATORIO DE DESEMPENHO]
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "performance.h"
#include "algorithms.h"
#include "storage.h"

#define BENCHMARK_SIZE 10000

static double elapsed_seconds(clock_t start, clock_t end) {
    return (double)(end - start) / (double)CLOCKS_PER_SEC;
}

int performance_run_report(const char *output_filename) {
    const size_t tamanho = BENCHMARK_SIZE;
    Book *acervo = (Book *)malloc(tamanho * sizeof(Book));
    Book *ins = NULL;
    Book *sel = NULL;
    if (!acervo) return -1;

    for (size_t i = 0; i < tamanho; ++i) {
        int codigo = (int)(((i * 37) + 1) % tamanho);
        char titulo[BOOK_TITLE_LEN];
        sprintf(titulo, "Livro %05zu", tamanho - i);
        book_init(&acervo[i], codigo, titulo, "Autor", 2020, 1);
    }

    clock_t seq_start = clock();
    volatile int seq_result = sequential_search_book_by_code(acervo, tamanho, 99999);
    clock_t seq_end = clock();

    /* Preparacao exigida pela busca binaria: vetor ordenado por codigo. */
    insertion_sort_books_by_code(acervo, tamanho);
    clock_t bin_start = clock();
    volatile int bin_result = binary_search_book_by_code(acervo, tamanho, 99999);
    clock_t bin_end = clock();

    ins = (Book *)malloc(tamanho * sizeof(Book));
    sel = (Book *)malloc(tamanho * sizeof(Book));
    if (!ins || !sel) {
        free(acervo); free(ins); free(sel); return -1;
    }

    /* Dados em ordem decrescente aproximada para tornar o benchmark representativo. */
    for (size_t i = 0; i < tamanho; ++i) {
        ins[i] = acervo[tamanho - 1 - i];
        sel[i] = ins[i];
    }

    clock_t ins_start = clock();
    insertion_sort_books_by_code(ins, tamanho);
    clock_t ins_end = clock();

    clock_t sel_start = clock();
    selection_sort_books_by_title(sel, tamanho);
    clock_t sel_end = clock();

    double seq_time = elapsed_seconds(seq_start, seq_end);
    double bin_time = elapsed_seconds(bin_start, bin_end);
    double ins_time = elapsed_seconds(ins_start, ins_end);
    double sel_time = elapsed_seconds(sel_start, sel_end);

    const char *nome = output_filename ? output_filename : "desempenho.txt";
    FILE *out = fopen(nome, "w");
    if (!out) {
        free(acervo); free(ins); free(sel); return -1;
    }

    fprintf(out, "RELATORIO DE DESEMPENHO - SGB Parte 2\n");
    fprintf(out, "======================================\n");
    fprintf(out, "Ambiente: clock() da libc (CLOCKS_PER_SEC = %ld)\n", (long)CLOCKS_PER_SEC);
    fprintf(out, "Quantidade sintetica de registros: %zu\n\n", tamanho);
    fprintf(out, "BUSCAS MANUAIS\n");
    fprintf(out, "Busca sequencial por codigo: %.6f s (indice %d)\n", seq_time, (int)seq_result);
    fprintf(out, "Busca binaria por codigo: %.6f s (indice %d)\n", bin_time, (int)bin_result);
    fprintf(out, "\nORDENACOES MANUAIS\n");
    fprintf(out, "Insertion Sort por codigo: %.6f s\n", ins_time);
    fprintf(out, "Selection Sort por titulo: %.6f s\n", sel_time);
    fprintf(out, "\nObservacao: dados sinteticos; o tempo pode variar conforme o sistema.\n");
    fclose(out);

    printf("Relatorio exportado para %s\n", nome);
    printf("Busca sequencial: %.6f s (indice %d)\n", seq_time, (int)seq_result);
    printf("Busca binaria: %.6f s (indice %d)\n", bin_time, (int)bin_result);
    printf("Insertion Sort: %.6f s\n", ins_time);
    printf("Selection Sort: %.6f s\n", sel_time);

    free(acervo); free(ins); free(sel);
    return 0;
}
