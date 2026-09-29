#ifndef LOAN_H
#define LOAN_H

#include <stdbool.h>
#include "book.h"
#include "user.h"

#define DATE_LEN 11 /* YYYY-MM-DD + '\0' */

typedef struct Loan {
    int book_id;
    int user_id;
    char date_loan[DATE_LEN];
    char date_return[DATE_LEN];
    bool returned;
} Loan;

void loan_init(void);
int loan_load_from_file(const char *path);
int loan_save_to_file(const char *path);
int loan_count_get(void);
int loan_issue(int book_id, int user_id, const char *date_loan);
int loan_return(int book_id, int user_id, const char *date_return);
void loan_list(void);
void loan_run(void);

#endif
