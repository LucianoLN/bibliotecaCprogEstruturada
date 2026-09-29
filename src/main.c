#include <stdio.h>

#include "../include/book.h"
#include "../include/loan.h"
#include "../include/menu.h"
#include "../include/user.h"

static void menu_clear_input(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

void menu_main(void) {
    int option = 0;

    do {
        puts("\n=== Biblioteca ===");
        puts("1. Gerenciar livros");
        puts("2. Gerenciar usuarios");
        puts("3. Gerenciar emprestimos");
        puts("0. Sair");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &option) != 1) {
            menu_clear_input();
            option = -1;
            continue;
        }
        menu_clear_input();

        switch (option) {
            case 1:
                book_run();
                break;
            case 2:
                user_run();
                break;
            case 3:
                loan_run();
                break;
            case 0:
                puts("Encerrando biblioteca.");
                break;
            default:
                puts("Opcao invalida.");
                break;
        }
    } while (option != 0);
}

int main(void) {
    menu_main();
    return 0;
}
