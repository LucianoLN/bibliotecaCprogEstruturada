#ifndef LOAN_H
#define LOAN_H

#include "book.h"
#include <stdbool.h>
#include "book.h"
#include "user.h"

#define DATE_LEN 11 /* YYYY-MM-DD + '\0' */

typedef struct Loan {
    int book_id;
    int user_id;
    char date_loan[DATE_LEN];
    char date_return[DATE_LEN];
    bool returned; /* false = not returned, true = returned */
} Loan;
/* Inicializa subsistema de empréstimos (carregar de arquivo se implementado) */
void loan_init(void);

/* Registra um empréstimo; retorna 0 em sucesso, <0 em erro */
int loan_issue(int book_id, int user_id, const char *date_loan);

/* Registra devolução; retorna 0 em sucesso, <0 se não encontrado */
int loan_return(int book_id, int user_id, const char *date_return);

/* Lista todos os empréstimos (para depuração/console) */
void loan_list(void);

#endif
