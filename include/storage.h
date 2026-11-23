/*
 * storage.h - Estado da biblioteca em memoria.
 *
 * [PARTE 1]                         vetor global estatico de livros.
 * [COMMIT 6 - ALOCACAO DINAMICA]  struct Library com tres vetores dinamicos
 *                                   (livros, usuarios, emprestimos) que crescem
 *                                   com realloc e sao liberados em lib_free.
 */
#ifndef STORAGE_H
#define STORAGE_H

#include <stddef.h>
#include <time.h>
#include "book.h"

#define USER_NAME_LEN 100
#define USER_COURSE_LEN 60

typedef struct User {
    int registration;               /* matricula (unica) */
    char name[USER_NAME_LEN];
    char course[USER_COURSE_LEN];
} User;

typedef struct Loan {
    int id;
    int book_id;
    int user_reg;
    time_t loan_date;
    time_t return_date;             /* 0 = ainda nao devolvido */
} Loan;

/* Cada vetor guarda quantos itens tem (n_) e quanto cabe sem realloc (cap_). */
typedef struct Library {
    Book *books;
    size_t n_books, cap_books;
    User *users;
    size_t n_users, cap_users;
    Loan *loans;
    size_t n_loans, cap_loans;
    int next_book_id;
    int next_loan_id;
} Library;

/* Erros das operacoes (sucesso e sempre >= 0). */
enum {
    LIB_ERR_NOMEM = -1,
    LIB_ERR_NO_BOOK = -2,
    LIB_ERR_NO_USER = -3,
    LIB_ERR_UNAVAILABLE = -4,
    LIB_ERR_DUPLICATE = -5,
    LIB_ERR_NO_LOAN = -6,
    LIB_ERR_INVALID = -7
};

void lib_init(Library *lib);
/* Libera todos os vetores (liberar_memoria) e deixa a struct vazia e reutilizavel. */
void lib_free(Library *lib);
const char *lib_strerror(int err);

/* Acrescentam um registro pronto (usado pela carga de arquivos). 0 ou LIB_ERR_NOMEM. */
int lib_push_book(Library *lib, const Book *b);
int lib_push_user(Library *lib, const User *u);
int lib_push_loan(Library *lib, const Loan *l);

/* Cadastro validado; devolve o codigo gerado do livro ou um erro. */
int lib_add_book(Library *lib, const char *title, const char *author, int year, int quantity);
/* Devolve 0 ou um erro (matricula repetida = LIB_ERR_DUPLICATE). */
int lib_add_user(Library *lib, int registration, const char *name, const char *course);
/* Devolve o codigo do emprestimo ou um erro. */
int lib_lend(Library *lib, int registration, int book_id, time_t when);
/* Devolve o indice do emprestimo encerrado em lib->loans ou um erro. */
int lib_return(Library *lib, int registration, int book_id, time_t when);

/* Busca sequencial (Parte 1); NULL se nao existir. */
Book *lib_find_book(const Library *lib, int id);
User *lib_find_user(const Library *lib, int registration);

#endif /* STORAGE_H */
