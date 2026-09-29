#include <stdio.h>
#include <string.h>

#include "../include/loan.h"
#include "../include/storage.h"

#define MAX_LOANS 256

static Loan loans[MAX_LOANS];
static int loan_count = 0;

static void loan_clear_input(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

void loan_init(void) {
    loan_count = 0;
}

int loan_count_get(void) {
    return loan_count;
}

int loan_load_from_file(const char *path) {
    int loaded = storage_load_loans(loans, MAX_LOANS, path ? path : "data/loans.txt");
    if (loaded < 0) {
        return -1;
    }
    loan_count = loaded;
    return loaded;
}

int loan_save_to_file(const char *path) {
    return storage_save_loans(loans, (size_t)loan_count, path ? path : "data/loans.txt");
}

int loan_issue(int book_id, int user_id, const char *date_loan) {
    if (loan_count >= MAX_LOANS) return -1;

    for (int i = 0; i < loan_count; ++i) {
        if (loans[i].book_id == book_id && loans[i].user_id == user_id && !loans[i].returned) {
            return -2;
        }
    }

    loans[loan_count].book_id = book_id;
    loans[loan_count].user_id = user_id;
    if (date_loan) {
        strncpy(loans[loan_count].date_loan, date_loan, DATE_LEN - 1);
        loans[loan_count].date_loan[DATE_LEN - 1] = '\0';
    } else {
        loans[loan_count].date_loan[0] = '\0';
    }
    loans[loan_count].date_return[0] = '\0';
    loans[loan_count].returned = false;
    loan_count++;
    printf("[loan] emitido: livro=%d usuario=%d\n", book_id, user_id);
    return 0;
}

int loan_return(int book_id, int user_id, const char *date_return) {
    for (int i = 0; i < loan_count; ++i) {
        if (loans[i].book_id == book_id && loans[i].user_id == user_id && loans[i].returned == false) {
            loans[i].returned = true;
            if (date_return) {
                strncpy(loans[i].date_return, date_return, DATE_LEN - 1);
                loans[i].date_return[DATE_LEN - 1] = '\0';
            }
            printf("[loan] devolvido: livro=%d usuario=%d\n", book_id, user_id);
            return 0;
        }
    }
    return -1;
}

void loan_list(void) {
    if (loan_count == 0) {
        puts("Nenhum empréstimo registrado.");
        return;
    }
    puts("Lista de empréstimos:");
    for (int i = 0; i < loan_count; ++i) {
        printf("%d) livro=%d usuario=%d data_loan=%s data_return=%s status=%s\n",
               i + 1,
               loans[i].book_id,
               loans[i].user_id,
               loans[i].date_loan[0] ? loans[i].date_loan : "-",
               loans[i].date_return[0] ? loans[i].date_return : "-",
               loans[i].returned ? "devolvido" : "emprestado");
    }
}

void loan_run(void) {
    loan_init();
    if (loan_load_from_file("data/loans.txt") < 0) {
        puts("Arquivo de emprestimos nao encontrado. Continuando com lista vazia.");
    }

    int option = 0;
    do {
        puts("\n=== Emprestimos ===");
        puts("1. Listar emprestimos");
        puts("2. Registrar emprestimo");
        puts("3. Registrar devolucao");
        puts("0. Voltar");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &option) != 1) {
            loan_clear_input();
            option = -1;
            continue;
        }
        loan_clear_input();

        switch (option) {
            case 1:
                loan_list();
                break;
            case 2: {
                int book_id;
                int user_id;
                char date[DATE_LEN];

                printf("ID do livro: ");
                if (scanf("%d", &book_id) != 1) {
                    loan_clear_input();
                    puts("ID do livro invalido.");
                    break;
                }
                loan_clear_input();

                printf("ID do usuario: ");
                if (scanf("%d", &user_id) != 1) {
                    loan_clear_input();
                    puts("ID do usuario invalido.");
                    break;
                }
                loan_clear_input();

                printf("Data do emprestimo (YYYY-MM-DD): ");
                if (!fgets(date, sizeof(date), stdin)) {
                    puts("Data invalida.");
                    break;
                }
                date[strcspn(date, "\r\n")] = '\0';

                Book *book = book_find_by_id(book_id);
                User *user = user_find_by_id(user_id);
                if (!book || !user) {
                    puts("Livro ou usuario nao encontrado.");
                    break;
                }
                if (book->quantity <= 0) {
                    puts("Livro indisponivel no estoque.");
                    break;
                }
                if (loan_issue(book_id, user_id, date) == 0) {
                    book_adjust_quantity(book_id, -1);
                    loan_save_to_file("data/loans.txt");
                    book_save_to_file("data/books.txt");
                    puts("Emprestimo registrado com sucesso.");
                } else {
                    puts("Nao foi possivel registrar o emprestimo.");
                }
                break;
            }
            case 3: {
                int book_id;
                int user_id;
                char date[DATE_LEN];

                printf("ID do livro: ");
                if (scanf("%d", &book_id) != 1) {
                    loan_clear_input();
                    puts("ID do livro invalido.");
                    break;
                }
                loan_clear_input();

                printf("ID do usuario: ");
                if (scanf("%d", &user_id) != 1) {
                    loan_clear_input();
                    puts("ID do usuario invalido.");
                    break;
                }
                loan_clear_input();

                printf("Data da devolucao (YYYY-MM-DD): ");
                if (!fgets(date, sizeof(date), stdin)) {
                    puts("Data invalida.");
                    break;
                }
                date[strcspn(date, "\r\n")] = '\0';

                if (loan_return(book_id, user_id, date) == 0) {
                    book_adjust_quantity(book_id, 1);
                    loan_save_to_file("data/loans.txt");
                    book_save_to_file("data/books.txt");
                    puts("Devolucao registrada com sucesso.");
                } else {
                    puts("Emprestimo nao encontrado para devolucao.");
                }
                break;
            }
            case 0:
                break;
            default:
                puts("Opcao invalida.");
                break;
        }
    } while (option != 0);
}
