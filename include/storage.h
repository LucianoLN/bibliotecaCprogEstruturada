#ifndef STORAGE_H
#define STORAGE_H

#include <stddef.h>
#include "book.h"
#include "loan.h"
#include "user.h"

int storage_load_users(User users[], size_t max_users, const char *path);
int storage_save_users(const User users[], size_t count, const char *path);

int storage_load_books(Book books[], size_t max_books, const char *path);
int storage_save_books(const Book books[], size_t count, const char *path);

int storage_load_loans(Loan loans[], size_t max_loans, const char *path);
int storage_save_loans(const Loan loans[], size_t count, const char *path);

#endif
