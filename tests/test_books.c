#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "fine.h"
#include "persist.h"
#include "search.h"
#include "sort.h"
#include "util.h"

#define TMP "tests/tmp_"

static void remove_tmp_files(void) {
    remove(TMP "livros.txt");
    remove(TMP "usuarios.txt");
    remove(TMP "emprestimos.txt");
}

static void test_dynamic_arrays(void) {
    Library lib;
    int i;
    lib_init(&lib);
    for (i = 1; i <= 100; i++) assert(lib_add_book(&lib, "Livro", "Autor", 2000, 1) == i);
    assert(lib.n_books == 100 && lib.cap_books >= 100);
    assert(lib_add_book(&lib, "", "Autor", 2000, 1) == LIB_ERR_INVALID);
    assert(lib_add_book(&lib, "A|B", "Autor", 2000, 1) == LIB_ERR_INVALID);
    assert(lib_add_book(&lib, "A", "Autor", -5, 1) == LIB_ERR_INVALID); 
    assert(lib_add_book(&lib, "A", "Autor", 2000, -1) == LIB_ERR_INVALID);
    lib_free(&lib);
    assert(lib.books == NULL && lib.n_books == 0 && lib.cap_books == 0);
}

static void test_loans(void) {
    Library lib;
    time_t today = date_today();
    lib_init(&lib);
    assert(lib_add_book(&lib, "C", "Kernighan", 1978, 1) == 1);
    assert(lib_add_user(&lib, 10, "Ana", "CC") == 0);
    assert(lib_add_user(&lib, 10, "Bia", "CC") == LIB_ERR_DUPLICATE);
    assert(lib_lend(&lib, 99, 1, today) == LIB_ERR_NO_USER);
    assert(lib_lend(&lib, 10, 99, today) == LIB_ERR_NO_BOOK);
    assert(lib_lend(&lib, 10, 1, today) == 1);
    assert(lib_find_book(&lib, 1)->quantity == 0);
    assert(lib_lend(&lib, 10, 1, today) == LIB_ERR_UNAVAILABLE);
    assert(lib_return(&lib, 10, 1, today) == 0);
    assert(lib_find_book(&lib, 1)->quantity == 1);
    assert(lib_return(&lib, 10, 1, today) == LIB_ERR_NO_LOAN);
    lib_free(&lib);
}

static void test_text_files(void) {
    Library lib, back;
    FILE *f;
    lib_init(&lib);
    lib_init(&back);
    lib_add_book(&lib, "Dom Casmurro", "Machado de Assis", 1899, 2);
    lib_add_user(&lib, 10, "Ana", "CC");
    lib_lend(&lib, 10, 1, date_today());

    assert(persist_save_txt(&lib, TMP) == 0);
    assert(persist_load_txt(&back, TMP) == 3);
    assert(back.n_books == 1 && back.books[0].quantity == 1);
    assert(back.loans[0].loan_date == lib.loans[0].loan_date && back.loans[0].return_date == 0);
    assert(back.next_book_id == 2 && back.next_loan_id == 2);

    remove_tmp_files();
    f = fopen(TMP "livros.txt", "w");
    assert(f);
    fprintf(f, "7|1949|1984|George Orwell\nlinha quebrada\n");
    fclose(f);
    assert(persist_load_txt(&back, TMP) == 1);
    assert(back.books[0].id == 7 && back.books[0].quantity == 1 && back.next_book_id == 8);
    assert(strcmp(back.books[0].title, "1984") == 0);

    remove_tmp_files();
    assert(persist_load_txt(&back, TMP) == PERSIST_NO_FILE);
    assert(back.n_books == 1);             
    lib_free(&lib);
    lib_free(&back);
}

static void test_binary_file(void) {
    const char *path = TMP "dados.bin";
    Library lib, back;
    FILE *f;
    int i;
    lib_init(&lib);
    lib_init(&back);
    for (i = 0; i < 20; i++) lib_add_book(&lib, "Livro", "Autor", 2000 + i, i);
    lib_add_user(&lib, 10, "Ana", "CC");
    lib_lend(&lib, 10, 5, date_today());

    assert(persist_save_bin(&lib, path) == 0);
    assert(persist_load_bin(&back, path) == 22);
    assert(back.n_books == 20 && back.n_users == 1 && back.n_loans == 1);
    assert(memcmp(back.books, lib.books, 20 * sizeof(Book)) == 0);
    assert(back.next_book_id == 21 && back.next_loan_id == 2);

    f = fopen(path, "wb");
    assert(f);
    fputs("lixo", f);
    fclose(f);
    assert(persist_load_bin(&back, path) == PERSIST_ERROR);
    assert(back.n_books == 20);       

    remove(path);
    assert(persist_load_bin(&back, path) == PERSIST_NO_FILE);
    lib_free(&lib);
    lib_free(&back);
}

static void test_sorting(void) {
    static const char *titles[] = {"pedra", "Ana", "zebra", "ana", "Mar", "pedra", "b"};
    void (*algos[2])(Book *, size_t, BookCmp) = {sort_insertion, sort_selection};
    Book a[7], b[7];
    size_t i;
    int k;
    for (i = 0; i < 7; i++) book_init(&a[i], (int)(70 - i * 7 % 10), titles[i], "x", 2000, 1);
    for (k = 0; k < 2; k++) {
        memcpy(b, a, sizeof a);
        algos[k](b, 7, book_cmp_title);
        assert(sort_is_sorted(b, 7, book_cmp_title));
        assert(str_icmp(b[0].title, "ana") == 0 && strcmp(b[6].title, "zebra") == 0);
        algos[k](b, 7, book_cmp_id);
        assert(sort_is_sorted(b, 7, book_cmp_id));
        algos[k](b, 0, book_cmp_id);
        algos[k](b, 1, book_cmp_id);
    }
    assert(!sort_is_sorted(a, 7, book_cmp_title));
}

static void test_binary_search(void) {
    Book v[40];
    char title[16];
    int n, i;
    for (n = 0; n <= 40; n++) {
        for (i = 0; i < n; i++) {
            snprintf(title, sizeof title, "t%03d", i);
            book_init(&v[i], i * 2 + 1, title, "x", 2000, 1); 
        }
        for (i = 0; i <= 2 * n + 1; i++) {
            snprintf(title, sizeof title, "T%03d", i);
            assert(search_bin_id(v, (size_t)n, i) == search_seq_id(v, (size_t)n, i));
            assert(search_bin_title(v, (size_t)n, title) == search_seq_title(v, (size_t)n, title));
        }
    }
}

static void test_fines(void) {
    Loan l;
    time_t t;
    memset(&l, 0, sizeof l);
    l.loan_date = date_make(25, 2, 2026);
    assert(date_diff_days(l.loan_date, loan_due_date(&l)) == LOAN_DAYS);
    assert(loan_due_date(&l) == date_make(4, 3, 2026));
    assert(loan_days_late(&l, date_make(4, 3, 2026)) == 0);    
    assert(loan_days_late(&l, date_make(5, 3, 2026)) == 1);
    assert(loan_fine_cents(&l, date_make(14, 3, 2026)) == 10 * FINE_CENTS_PER_DAY);
    l.return_date = date_make(6, 3, 2026);          
    assert(loan_days_late(&l, date_make(31, 12, 2026)) == 2);
    l.loan_date = date_make(28, 12, 2026);
    l.return_date = date_make(10, 1, 2027);
    assert(loan_days_late(&l, 0) == 6);

    assert(date_parse("05/03/2026", &t) && t == date_make(5, 3, 2026));
    assert(date_parse("5/3/2026 ", &t));
    assert(!date_parse("31/02/2026", &t) && !date_parse("5/3", &t));
    assert(!date_parse("01/01/2026x", &t) && !date_parse("", &t));
}

int main(void) {
    test_fines();
    test_sorting();
    test_binary_search();
    test_dynamic_arrays();
    test_loans();
    test_text_files();
    test_binary_file();
    printf("Todos os testes passaram.\n");
    return 0;
}
