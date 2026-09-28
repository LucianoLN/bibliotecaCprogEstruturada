#ifndef STORAGE_H
#define STORAGE_H

#include "book.h"
#include "user.h"
#include "loan.h"

/* Simple persistence layer prototypes (text files, CSV or simple format)
   Implementations should follow the project's constraints: only C stdlib. */

int storage_load_books(const char *path);
int storage_save_books(const char *path);

int storage_load_users(const char *path);
int storage_save_users(const char *path);

int storage_load_loans(const char *path);
int storage_save_loans(const char *path);

#endif
