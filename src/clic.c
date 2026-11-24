#include <stdio.h>
#include <string.h>
#include "cli.h"
#include "fine.h"
#include "persist.h"
#include "report.h"
#include "search.h"
#include "sort.h"
#include "util.h"

#define LINE_LEN 256

/* ---------- Entrada ---------- */

static int read_line(const char *prompt, char *buf, size_t size) {
    printf("%s", prompt);
    fflush(stdout);

    if (!fgets(buf, (int)size, stdin)) {
        buf[0] = '\0';
        return 0;
    }

    if (!strchr(buf, '\n')) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
    }

    buf[strcspn(buf, "\r\n")] = '\0';
    return 1;
}

static int read_int(const char *prompt, int *out) {
    char buf[32];

    if (!read_line(prompt, buf, sizeof buf)) return 0;
    if (parse_int(buf, out)) return 1;

    printf("Numero invalido.\n");
    return 0;
}

static void report_error(int rc) {
    printf("Erro: %s.\n", lib_strerror(rc));
}

/* ---------- Impressao ---------- */

static void print_book_header(void) {
    printf("%-6s %-35s %-25s %5s %5s\n",
           "Codigo", "Titulo", "Autor", "Ano", "Disp.");
}

static void print_book(const Book *b) {
    printf("%-6d %-35.35s %-25.25s %5d %5d\n",
           b->id, b->title, b->author, b->year, b->quantity);
}

static void print_loan_header(void) {
    printf("%-4s %-8s %-5s %-22s %-10s %-10s %-10s %10s\n",
           "Id", "Matric.", "Livro", "Titulo", "Emprestimo", "Prazo", "Devolucao", "Multa");
}

/* ---------- Livros ---------- */

static void add_book(Library *lib) {
    char title[LINE_LEN], author[LINE_LEN];
    int year, quantity, rc;

    read_line("Titulo: ", title, sizeof title);
    read_line("Autor: ", author, sizeof author);

    if (!read_int("Ano: ", &year) || !read_int("Quantidade disponivel: ", &quantity)) {
        return;
    }

    rc = lib_add_book(lib, title, author, year, quantity);
    if (rc < 0) {
        report_error(rc);
    } else {
        printf("Livro cadastrado com codigo %d.\n", rc);
    }
}

static void list_books(Library *lib) {
    size_t i;

    if (lib->n_books == 0) {
        printf("(nenhum livro cadastrado)\n");
        return;
    }

    print_book_header();
    for (i = 0; i < lib->n_books; i++) {
        print_book(&lib->books[i]);
    }
}

static void search_books(Library *lib) {
    char term[LINE_LEN];
    size_t i;
    unsigned long found = 0;

    read_line("Trecho do titulo ou do autor: ", term, sizeof term);

    for (i = 0; i < lib->n_books; i++) {
        const Book *b = &lib->books[i];
        if (str_icontains(b->title, term) || str_icontains(b->author, term)) {
            if (found++ == 0) print_book_header();
            print_book(b);
        }
    }

    printf("%lu livro(s) encontrado(s).\n", found);
}


/* [COMMIT 8] Pergunta o criterio; NULL se a opcao for invalida. */
static BookCmp read_criterion(void) {
    int field;
    if (!read_int("Campo (1 = codigo, 2 = titulo): ", &field)) return NULL;
    if (field == 1) return book_cmp_id;
    if (field == 2) return book_cmp_title;
    printf("Campo invalido.\n");
    return NULL;
}

/* [COMMIT 8] Ordena o proprio vetor do acervo (a ordem fica salva no .bin). */
static void sort_books(Library *lib) {
    int algo;
    BookCmp cmp;
    if (!read_int("Algoritmo (1 = Insertion Sort, 2 = Selection Sort): ", &algo)) return;
    if (algo != 1 && algo != 2) {
        printf("Algoritmo invalido.\n");
        return;
    }
    if (!(cmp = read_criterion())) return;
    if (algo == 1) sort_insertion(lib->books, lib->n_books, cmp);
    else sort_selection(lib->books, lib->n_books, cmp);
    printf("Acervo ordenado com %s.\n", algo == 1 ? "Insertion Sort" : "Selection Sort");
    list_books(lib);
}


/* [COMMIT 9] Busca binaria. Ela so funciona em vetor ordenado pelo mesmo
 * campo, entao confere antes e, se preciso, ordena com Insertion Sort. */
static void binary_search_books(Library *lib) {
    char title[LINE_LEN];
    int id = 0, idx;
    BookCmp cmp = read_criterion();
    if (!cmp) return;
    if (cmp == book_cmp_id) {
        if (!read_int("Codigo: ", &id)) return;
    } else {
        read_line("Titulo exato: ", title, sizeof title);
    }
    if (!sort_is_sorted(lib->books, lib->n_books, cmp)) {
        sort_insertion(lib->books, lib->n_books, cmp);
        printf("(acervo nao estava ordenado por %s; ordenado agora com Insertion Sort)\n",
               cmp == book_cmp_id ? "codigo" : "titulo");
    }
    idx = cmp == book_cmp_id ? search_bin_id(lib->books, lib->n_books, id)
                             : search_bin_title(lib->books, lib->n_books, title);
    if (idx < 0) {
        printf("Livro nao encontrado.\n");
        return;
    }
    print_book_header();
    print_book(&lib->books[idx]);
}


/* ---------- Usuarios ---------- */

static void add_user(Library *lib) {
    char name[LINE_LEN], course[LINE_LEN];
    int reg, rc;

    if (!read_int("Matricula: ", &reg)) return;

    read_line("Nome: ", name, sizeof name);
    read_line("Curso: ", course, sizeof course);

    rc = lib_add_user(lib, reg, name, course);
    if (rc < 0) {
        report_error(rc);
    } else {
        printf("Usuario cadastrado.\n");
    }
}

static void list_users(Library *lib) {
    size_t i;

    if (lib->n_users == 0) {
        printf("(nenhum usuario cadastrado)\n");
        return;
    }

    printf("%-10s %-35s %s\n", "Matricula", "Nome", "Curso");
    for (i = 0; i < lib->n_users; i++) {
        const User *u = &lib->users[i];
        printf("%-10d %-35.35s %s\n", u->registration, u->name, u->course);
    }
}

static void find_user(Library *lib) {
    int reg;
    const User *u;

    if (!read_int("Matricula: ", &reg)) return;

    u = lib_find_user(lib, reg);
    if (!u) {
        printf("Usuario nao encontrado.\n");
    } else {
        printf("%d - %s (%s)\n", u->registration, u->name, u->course);
    }
}

/* ---------- Emprestimos ---------- */

static void lend_book(Library *lib) {
    int reg, book_id, rc;
    time_t when;
    char due[16];

    if (!read_int("Matricula: ", &reg) ||
        !read_int("Codigo do livro: ", &book_id) ||
        !read_date("Data do emprestimo (dd/mm/aaaa, Enter = hoje): ", &when)) {
        return;
    }

    rc = lib_lend(lib, reg, book_id, when);
    if (rc < 0) {
        report_error(rc);
        return;
    }

    date_format(loan_due_date(&lib->loans[lib->n_loans - 1]), due, sizeof due);
    printf("Emprestimo %d registrado. Devolver ate %s.\n", rc, due);
}


/* [COMMIT 10 - HISTORICO] Todos os emprestimos de um usuario, abertos e
 * devolvidos, na ordem em que aconteceram, com os totais no fim. */
static void user_history(Library *lib) {
    int reg;
    size_t i;
    unsigned long total = 0, open = 0;
    long fines = 0;                 /* [COMMIT 11] */
    char fines_txt[24];
    const User *u;
    if (!read_int("Matricula: ", &reg)) return;
    if (!(u = lib_find_user(lib, reg))) {
        printf("Usuario nao encontrado.\n");
        return;
    }
    printf("Historico de %s (matricula %d, %s)\n", u->name, u->registration, u->course);
    for (i = 0; i < lib->n_loans; i++) {
        const Loan *l = &lib->loans[i];
        if (l->user_reg != reg) continue;
        if (total++ == 0) print_loan_header();
        print_loan(lib, l);
        if (!l->return_date) open++;
        fines += loan_fine_cents(l, date_today());
    }
    if (total == 0) printf("(nenhum emprestimo)\n");
    fine_format(fines, fines_txt, sizeof fines_txt);
    printf("Total: %lu emprestimo(s), %lu em aberto, %lu devolvido(s). Multas: %s.\n",
           total, open, total - open, fines_txt);
}

/* ---------- Arquivos ---------- */

static void save_now(Library *lib) {
    if (persist_save_bin(lib, BIN_FILE) == 0) {
        printf("Dados salvos em %s.\n", BIN_FILE);
    } else {
        printf("Erro ao salvar os dados.\n");
    }
}

static void export_txt(Library *lib) {
    if (persist_save_txt(lib, "") == 0) {
        printf("Exportado para livros.txt, usuarios.txt e emprestimos.txt.\n");
    } else {
        printf("Erro ao exportar.\n");
    }
}

/* ---------- Menu ---------- */

typedef struct MenuItem {
    const char *label;
    void (*run)(Library *lib);
} MenuItem;

static const MenuItem MENU[] = {
    {"Acervo",             "Cadastrar livro",                           add_book},
    {NULL,                 "Listar livros",                             list_books},
    {NULL,                 "Buscar livro por titulo/autor (sequencial)", search_books},
    {"Usuarios",           "Cadastrar usuario",                         add_user},
    {NULL,                 "Listar usuarios",                           list_users},
    {NULL,                 "Buscar usuario por matricula",              find_user},
    {"Emprestimos",        "Registrar emprestimo",                      lend_book},
    {NULL,                 "Registrar devolucao",                       return_book},
    {NULL,                 "Listar emprestimos",                        list_loans},
    {"Dados e relatorios", "Salvar dados (.bin)",                       save_now},
    {NULL,                 "Exportar dados para texto (.txt)",          export_txt},
};

#define MENU_LEN ((int)(sizeof MENU / sizeof MENU[0]))

void cli_run(Library *lib) {
    char buf[32];
    int choice, i;

    for (;;) {
        printf("\n========== SGB - Biblioteca Universitaria ==========\n");
        printf("%lu livro(s) | %lu usuario(s) | %lu emprestimo(s)\n",
               (unsigned long)lib->n_books,
               (unsigned long)lib->n_users,
               (unsigned long)lib->n_loans);

        for (i = 0; i < MENU_LEN; i++) {
            if (MENU[i].section) printf("\n[%s]\n", MENU[i].section);
            printf("  %2d) %s\n", i + 1, MENU[i].label);
        }

        printf("\n   0) Sair (salva automaticamente)\n");

        if (!read_line("Escolha: ", buf, sizeof buf)) break;

        if (!parse_int(buf, &choice) || choice < 0 || choice > MENU_LEN) {
            printf("Opcao invalida.\n");
            continue;
        }

        if (choice == 0) break;
        MENU[choice - 1].run(lib);
    }
}
