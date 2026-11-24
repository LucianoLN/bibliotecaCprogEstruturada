/*
 * book.h - Modelo de livro do acervo.
 *
 * [PARTE 1]                         estrutura Book e book_init.
 * [COMMIT 6 - ALOCACAO DINAMICA]  campo quantity (exemplares disponiveis).
 * [COMMIT 8 - ORDENACAO]          comparadores por codigo e por titulo.
 */
#ifndef BOOK_H
#define BOOK_H

#define BOOK_TITLE_LEN 128
#define BOOK_AUTHOR_LEN 128

typedef struct Book {
    int id;                         /* codigo do livro (gerado automaticamente) */
    char title[BOOK_TITLE_LEN];
    char author[BOOK_AUTHOR_LEN];
    int year;
    int quantity;                   /* [COMMIT 6] exemplares disponiveis */
} Book;

/* [PARTE 1] Preenche um Book, truncando textos longos com seguranca. */
void book_init(Book *b, int id, const char *title, const char *author,
               int year, int quantity);

/* [COMMIT 8] Criterios de ordenacao (<0, 0, >0 como strcmp). */
int book_cmp_id(const Book *a, const Book *b);
/* Titulo sem diferenciar maiusculas; empate desempata pelo codigo. */
int book_cmp_title(const Book *a, const Book *b);

#endif /* BOOK_H */
