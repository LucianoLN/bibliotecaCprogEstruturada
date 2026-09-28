#include <stdio.h>
#include <string.h>
#include "../include/loan.h"

#define MAX_LOANS 256

static Loan loans[MAX_LOANS];
static int loan_count = 0;

void loan_init(void) {
    loan_count = 0;
}

int loan_issue(int book_id, int user_id, const char *date_loan) {
    if (loan_count >= MAX_LOANS) return -1;
    loans[loan_count].book_id = book_id;
    loans[loan_count].user_id = user_id;
    if (date_loan) {
        strncpy(loans[loan_count].date_loan, date_loan, DATE_LEN - 1);
        loans[loan_count].date_loan[DATE_LEN - 1] = '\0';
    } else {
        loans[loan_count].date_loan[0] = '\0';
    }
    loans[loan_count].date_return[0] = '\0';
    loans[loan_count].returned = false;
    loan_count++;
    printf("[loan] emitido: livro=%d usuario=%d\n", book_id, user_id);
    return 0;
}

int loan_return(int book_id, int user_id, const char *date_return) {
    for (int i = 0; i < loan_count; ++i) {
        if (loans[i].book_id == book_id && loans[i].user_id == user_id && loans[i].returned == false) {
            loans[i].returned = true;
            if (date_return) {
                strncpy(loans[i].date_return, date_return, DATE_LEN - 1);
                loans[i].date_return[DATE_LEN - 1] = '\0';
            }
            printf("[loan] devolvido: livro=%d usuario=%d\n", book_id, user_id);
            return 0;
        }
    }
    return -1; /* não encontrado */
}

void loan_list(void) {
    if (loan_count == 0) {
        puts("Nenhum empréstimo registrado.");
        return;
    }
    puts("Lista de empréstimos:");
    for (int i = 0; i < loan_count; ++i) {
        printf("%d) livro=%d usuario=%d data_loan=%s data_return=%s status=%s\n",
               i + 1,
               loans[i].book_id,
               loans[i].user_id,
               loans[i].date_loan[0] ? loans[i].date_loan : "-",
               loans[i].date_return[0] ? loans[i].date_return : "-",
               loans[i].returned ? "devolvido" : "emprestado");
    }
}
