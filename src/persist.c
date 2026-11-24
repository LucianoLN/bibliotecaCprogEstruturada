#include <stdio.h>
#include <string.h>
#include "persist.h"
#include "util.h"

#define TXT_BOOKS  "livros.txt"
#define TXT_USERS  "usuarios.txt"
#define TXT_LOANS  "emprestimos.txt"
#define MAX_FIELDS 5
#define LINE_LEN   512

static FILE *open_file(const char *prefix, const char *name, const char *mode) {
    char path[512];
    snprintf(path, sizeof path, "%s%s", prefix, name);
    return fopen(path, mode);
}

int persist_save_txt(const Library *lib, const char *prefix) {
    FILE *fb = open_file(prefix, TXT_BOOKS, "w");
    FILE *fu = open_file(prefix, TXT_USERS, "w");
    FILE *fl = open_file(prefix, TXT_LOANS, "w");
    int ok = fb && fu && fl;
    size_t i;

    for (i = 0; ok && i < lib->n_books; i++) {
        const Book *b = &lib->books[i];
        ok = fprintf(fb, "%d|%d|%d|%s|%s\n",
                     b->id, b->year, b->quantity, b->title, b->author) > 0;
    }

    for (i = 0; ok && i < lib->n_users; i++) {
        const User *u = &lib->users[i];
        ok = fprintf(fu, "%d|%s|%s\n",
                     u->registration, u->name, u->course) > 0;
    }

    for (i = 0; ok && i < lib->n_loans; i++) {
        const Loan *l = &lib->loans[i];
        ok = fprintf(fl, "%d|%d|%d|%d|%d\n",
                     l->id, l->book_id, l->user_reg,
                     date_to_ymd(l->loan_date), date_to_ymd(l->return_date)) > 0;
    }

    if (fb && fclose(fb) != 0) ok = 0;
    if (fu && fclose(fu) != 0) ok = 0;
    if (fl && fclose(fl) != 0) ok = 0;

    return ok ? 0 : PERSIST_ERROR;
}
