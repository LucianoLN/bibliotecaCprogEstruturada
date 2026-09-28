#include <stdio.h>
#include "../include/loan.h"

void loan_init(void) {
    /* initialize loan subsystem (load data etc.) */
}

int loan_issue(int book_id, int user_id) {
    (void)book_id; (void)user_id;
    printf("[loan] issue placeholder\n");
    return 0;
}

int loan_return(int book_id, int user_id) {
    (void)book_id; (void)user_id;
    printf("[loan] return placeholder\n");
    return 0;
}

void loan_list(void) {
    printf("[loan] list placeholder\n");
}
