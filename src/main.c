#include <stdio.h>
#include <stdlib.h>
#include "../include/menu.h"
#include "../include/storage.h"

static void print_menu(void) {
    puts("--- Biblioteca (esqueleto) ---");
    puts("1) Livros");
    puts("2) Usuários");
    puts("3) Empréstimos / Devoluções");
    puts("0) Sair");
}

void menu_run(void) {
    int opt = -1;
    while (opt != 0) {
        print_menu();
        if (scanf("%d", &opt) != 1) { break; }
        switch (opt) {
            case 1:
                puts("Entrou em Livros (placeholder)");
                break;
            case 2:
                puts("Entrou em Usuários (placeholder)");
                break;
            case 3:
                puts("Entrou em Empréstimos (placeholder)");
                break;
            case 0:
                puts("Saindo...");
                break;
            default:
                puts("Opção inválida");
        }
    }
}

int main(void) {
    /* Carregar persistência - implementações pendentes */
    storage_load_books("data/books.txt");
    storage_load_users("data/users.txt");
    storage_load_loans("data/loans.txt");

    menu_run();

    /* Salvar antes de sair */
    storage_save_books("data/books.txt");
    storage_save_users("data/users.txt");
    storage_save_loans("data/loans.txt");

    return 0;
}
