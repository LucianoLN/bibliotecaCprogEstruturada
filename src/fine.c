/*
 * fine.c - Prazo de devolucao, atraso e multa dos emprestimos.
 *
 * [COMMIT 11 - ATRASOS E MULTA]   regras de prazo e multa por dia.
 */
#include <stdio.h>
#include "fine.h"
#include "util.h"

time_t loan_due_date(const Loan *l) {
    return date_add_days(l->loan_date, LOAN_DAYS);
}

int loan_days_late(const Loan *l, time_t today) {
    time_t end = l->return_date ? l->return_date : today;
    int late = date_diff_days(loan_due_date(l), end);
    return late > 0 ? late : 0;
}

int loan_fine_cents(const Loan *l, time_t today) {
    return loan_days_late(l, today) * FINE_CENTS_PER_DAY;
}

void fine_format(long cents, char *buf, size_t size) {
    snprintf(buf, size, "R$ %ld,%02ld", cents / 100, cents % 100);
}
