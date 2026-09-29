#include <stdio.h>
#include <string.h>

#include "../include/book.h"
#include "../include/storage.h"

#define MAX_BOOKS 512

static Book books[MAX_BOOKS];
static int book_count = 0;

static void book_clear_input(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

void book_init(void) {
    book_count = 0;
}

int book_count_get(void) {
    return book_count;
}

int book_load_from_file(const char *path) {
    int loaded = storage_load_books(books, MAX_BOOKS, path ? path : "data/books.txt");
    if (loaded < 0) {
        return -1;
    }
    book_count = loaded;
    return loaded;
}

int book_save_to_file(const char *path) {
    return storage_save_books(books, (size_t)book_count, path ? path : "data/books.txt");
}

int book_add(const Book *b) {
    if (!b) return -1;
    if (book_count >= MAX_BOOKS) return -2;
    if (b->title[0] == '\0' || b->author[0] == '\0') return -3;
    for (int i = 0; i < book_count; ++i) {
        if (books[i].id == b->id) return -4;
    }
    books[book_count] = *b;
    book_count++;
    return 0;
}

Book *book_find_by_id(int id) {
    for (int i = 0; i < book_count; ++i) {
        if (books[i].id == id) return &books[i];
    }
    return NULL;
}

Book *book_find_by_title(const char *title) {
    if (!title) return NULL;
    for (int i = 0; i < book_count; ++i) {
        if (strstr(books[i].title, title) != NULL) return &books[i];
    }
    return NULL;
}

void book_list(void) {
    if (book_count == 0) {
        puts("Nenhum livro cadastrado.");
        return;
    }
    puts("Livros cadastrados:");
    for (int i = 0; i < book_count; ++i) {
        printf("%d) id=%d titulo=%s autor=%s ano=%d qtd=%d\n",
               i + 1,
               books[i].id,
               books[i].title,
               books[i].author,
               books[i].year,
               books[i].quantity);
    }
}

int book_adjust_quantity(int id, int delta) {
    Book *b = book_find_by_id(id);
    if (!b) return -1;
    if (delta < 0 && b->quantity + delta < 0) return -2;
    b->quantity += delta;
    return 0;
}

void book_run(void) {
    book_init();
    if (book_load_from_file("data/books.txt") < 0) {
        puts("Arquivo de livros nao encontrado. Continuando com lista vazia.");
    }

    int option = 0;
    do {
        puts("\n=== Livros ===");
        puts("1. Listar livros");
        puts("2. Cadastrar livro");
        puts("3. Buscar livro por ID");
        puts("4. Buscar livro por titulo");
        puts("0. Voltar");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &option) != 1) {
            book_clear_input();
            option = -1;
            continue;
        }
        book_clear_input();

        switch (option) {
            case 1:
                book_list();
                break;
            case 2: {
                Book b;
                char title[BOOK_TITLE_LEN];
                char author[BOOK_AUTHOR_LEN];

                printf("ID: ");
                if (scanf("%d", &b.id) != 1) {
                    book_clear_input();
                    puts("ID invalido.");
                    break;
                }
                book_clear_input();

                printf("Titulo: ");
                if (!fgets(title, sizeof(title), stdin)) {
                    puts("Titulo invalido.");
                    break;
                }
                title[strcspn(title, "\r\n")] = '\0';
                snprintf(b.title, sizeof(b.title), "%s", title);

                printf("Autor: ");
                if (!fgets(author, sizeof(author), stdin)) {
                    puts("Autor invalido.");
                    break;
                }
                author[strcspn(author, "\r\n")] = '\0';
                snprintf(b.author, sizeof(b.author), "%s", author);

                printf("Ano: ");
                if (scanf("%d", &b.year) != 1) {
                    book_clear_input();
                    puts("Ano invalido.");
                    break;
                }
                book_clear_input();

                printf("Quantidade: ");
                if (scanf("%d", &b.quantity) != 1) {
                    book_clear_input();
                    puts("Quantidade invalida.");
                    break;
                }
                book_clear_input();

                if (book_add(&b) == 0) {
                    book_save_to_file("data/books.txt");
                    puts("Livro cadastrado com sucesso.");
                } else {
                    puts("Erro ao cadastrar livro. Verifique ID, dados ou capacidade.");
                }
                break;
            }
            case 3: {
                int id;
                printf("Informe o ID: ");
                if (scanf("%d", &id) != 1) {
                    book_clear_input();
                    puts("ID invalido.");
                    break;
                }
                book_clear_input();

                Book *found = book_find_by_id(id);
                if (!found) {
                    puts("Livro nao encontrado.");
                } else {
                    printf("Livro encontrado: id=%d titulo=%s autor=%s ano=%d qtd=%d\n",
                           found->id,
                           found->title,
                           found->author,
                           found->year,
                           found->quantity);
                }
                break;
            }
            case 4: {
                char title[BOOK_TITLE_LEN];
                printf("Informe o titulo ou parte do titulo: ");
                if (!fgets(title, sizeof(title), stdin)) {
                    puts("Titulo invalido.");
                    break;
                }
                title[strcspn(title, "\r\n")] = '\0';

                Book *found = book_find_by_title(title);
                if (!found) {
                    puts("Livro nao encontrado.");
                } else {
                    printf("Livro encontrado: id=%d titulo=%s autor=%s ano=%d qtd=%d\n",
                           found->id,
                           found->title,
                           found->author,
                           found->year,
                           found->quantity);
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

