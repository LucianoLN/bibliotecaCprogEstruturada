#ifndef LOAN_H
#define LOAN_H

#include "book.h"
#include "user.h"

#define DATE_LEN 11 /* YYYY-MM-DD + null */

typedef struct Loan {
    int book_id;
    int user_id;
    char date_loan[DATE_LEN];
    char date_return[DATE_LEN];
    int returned; /* 0 = not returned, 1 = returned */
} Loan;

void loan_init(void);
int loan_issue(int book_id, int user_id);
int loan_return(int book_id, int user_id);
void loan_list(void);

#endif
