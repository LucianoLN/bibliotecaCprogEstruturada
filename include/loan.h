#ifndef LOAN_H
#define LOAN_H

#include <time.h>

#define LOAN_STATUS_ACTIVE 0
#define LOAN_STATUS_RETURNED 1

typedef struct Loan {
    int id;
    int matricula;
    int book_code;
    time_t loan_date;
    time_t due_date;
    time_t return_date;
    int status;
} Loan;

#endif
