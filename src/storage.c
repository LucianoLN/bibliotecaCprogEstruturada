#include <stdlib.h>
#include <string.h>
#include "search.h"
#include "storage.h"
#include "util.h"

static void *grow(void *arr, size_t *cap, size_t count, size_t elem_size) {
    size_t new_cap;
    void *p;
    if (count < *cap) return arr;
    new_cap = *cap ? *cap * 2 : 8;
    p = realloc(arr, new_cap * elem_size);
    if (p) *cap = new_cap;
    return p;
}

void lib_init(Library *lib) {
    memset(lib, 0, sizeof *lib);
    lib->next_book_id = 1;
    lib->next_loan_id = 1;
}

void lib_free(Library *lib) {
    free(lib->books);
    free(lib->users);
    free(lib->loans);
    lib_init(lib);
}

const char *lib_strerror(int err) {
    switch (err) {
    case LIB_ERR_NOMEM:       return "memoria insuficiente";
    case LIB_ERR_NO_BOOK:     return "livro nao encontrado";
    case LIB_ERR_NO_USER:     return "usuario nao encontrado";
    case LIB_ERR_UNAVAILABLE: return "nenhum exemplar disponivel";
    case LIB_ERR_DUPLICATE:   return "matricula ja cadastrada";
    case LIB_ERR_NO_LOAN:     return "nao ha emprestimo em aberto desse livro para esse usuario";
    case LIB_ERR_INVALID:     return "dados invalidos";
    default:                  return "erro desconhecido";
    }
}

int lib_push_book(Library *lib, const Book *b) {
    Book *v = grow(lib->books, &lib->cap_books, lib->n_books, sizeof *v);
    if (!v) return LIB_ERR_NOMEM;
    lib->books = v;
    v[lib->n_books++] = *b;
    if (b->id >= lib->next_book_id) lib->next_book_id = b->id + 1;
    return 0;
}

int lib_push_user(Library *lib, const User *u) {
    User *v = grow(lib->users, &lib->cap_users, lib->n_users, sizeof *v);
    if (!v) return LIB_ERR_NOMEM;
    lib->users = v;
    v[lib->n_users++] = *u;
    return 0;
}

int lib_push_loan(Library *lib, const Loan *l) {
    Loan *v = grow(lib->loans, &lib->cap_loans, lib->n_loans, sizeof *v);
    if (!v) return LIB_ERR_NOMEM;
    lib->loans = v;
    v[lib->n_loans++] = *l;
    if (l->id >= lib->next_loan_id) lib->next_loan_id = l->id + 1;
    return 0;
}

static int valid_text(const char *s) {
    return s && *s && !strchr(s, '|');
}

int lib_add_book(Library *lib, const char *title, const char *author, int year, int quantity) {
    Book b;
    int rc;
    book_init(&b, lib->next_book_id, title, author, year, quantity);
    rc = lib_push_book(lib, &b);
    return rc < 0 ? rc : b.id;
}

int lib_add_user(Library *lib, int registration, const char *name, const char *course) {
    User u;
    if (registration <= 0 || !valid_text(name) || !valid_text(course)) return LIB_ERR_INVALID;
    if (lib_find_user(lib, registration)) return LIB_ERR_DUPLICATE;
    memset(&u, 0, sizeof u);
    u.registration = registration;
    str_copy(u.name, name, sizeof u.name);
    str_copy(u.course, course, sizeof u.course);
    return lib_push_user(lib, &u);
}

int lib_lend(Library *lib, int registration, int book_id, time_t when) {
    Book *b = lib_find_book(lib, book_id);
    Loan l;
    int rc;
    if (!lib_find_user(lib, registration)) return LIB_ERR_NO_USER;
    if (!b) return LIB_ERR_NO_BOOK;
    if (b->quantity <= 0) return LIB_ERR_UNAVAILABLE;
    memset(&l, 0, sizeof l);
    l.id = lib->next_loan_id;
    l.book_id = book_id;
    l.user_reg = registration;
    l.loan_date = when;
    l.return_date = 0;
    rc = lib_push_loan(lib, &l);  
    if (rc < 0) return rc;
    b->quantity--;
    return l.id;
}

int lib_return(Library *lib, int registration, int book_id, time_t when) {
    size_t i;
    for (i = 0; i < lib->n_loans; i++) {
        Loan *l = &lib->loans[i];
        if (l->user_reg == registration && l->book_id == book_id && l->return_date == 0) {
            Book *b = lib_find_book(lib, book_id);
            if (when < l->loan_date) return LIB_ERR_INVALID;
            l->return_date = when;
            if (b) b->quantity++;
            return (int)i;
        }
    }
    return LIB_ERR_NO_LOAN;
}

Book *lib_find_book(const Library *lib, int id) {
    int i = search_seq_id(lib->books, lib->n_books, id);
    return i < 0 ? NULL : &lib->books[i];
}

User *lib_find_user(const Library *lib, int registration) {
    size_t i;
    for (i = 0; i < lib->n_users; i++)
        if (lib->users[i].registration == registration) return &lib->users[i];
    return NULL;
}
