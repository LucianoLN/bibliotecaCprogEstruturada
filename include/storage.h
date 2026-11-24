#ifndef STORAGE_H
#define STORAGE_H

#include <stddef.h>
#include <time.h>
#include "book.h"

#define USER_NAME_LEN 100
#define USER_COURSE_LEN 60

typedef struct User {
    int registration;              
    char name[USER_NAME_LEN];
    char course[USER_COURSE_LEN];
} User;

typedef struct Loan {
    int id;
    int book_id;
    int user_reg;
    time_t loan_date;
    time_t return_date;             
} Loan;

typedef struct Library {
    Book *books;
    size_t n_books, cap_books;
    User *users;
    size_t n_users, cap_users;
    Loan *loans;
    size_t n_loans, cap_loans;
    int next_book_id;
    int next_loan_id;
} Library;

enum {
    LIB_ERR_NOMEM = -1,
    LIB_ERR_NO_BOOK = -2,
    LIB_ERR_NO_USER = -3,
    LIB_ERR_UNAVAILABLE = -4,
    LIB_ERR_DUPLICATE = -5,
    LIB_ERR_NO_LOAN = -6,
    LIB_ERR_INVALID = -7
};

void lib_init(Library *lib);
void lib_free(Library *lib);
const char *lib_strerror(int err);

int lib_push_book(Library *lib, const Book *b);
int lib_push_user(Library *lib, const User *u);
int lib_push_loan(Library *lib, const Loan *l);

int lib_add_book(Library *lib, const char *title, const char *author, int year, int quantity);

int lib_add_user(Library *lib, int registration, const char *name, const char *course);

int lib_lend(Library *lib, int registration, int book_id, time_t when);

int lib_return(Library *lib, int registration, int book_id, time_t when);

Book *lib_find_book(const Library *lib, int id);
User *lib_find_user(const Library *lib, int registration);

#endif
