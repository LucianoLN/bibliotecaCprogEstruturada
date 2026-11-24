#ifndef FINE_H
#define FINE_H

#include <stddef.h>
#include <time.h>
#include "storage.h"

#define LOAN_DAYS 7                
#define FINE_CENTS_PER_DAY 100   

time_t loan_due_date(const Loan *l);

int loan_days_late(const Loan *l, time_t today);

int loan_fine_cents(const Loan *l, time_t today);

void fine_format(long cents, char *buf, size_t size);

#endif
