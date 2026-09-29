#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/storage.h"
#include "../include/user.h"

#define MAX_USERS 512

static User users[MAX_USERS];
static int user_count = 0;

void user_init(void) {
    user_count = 0;
}

int user_count_get(void) {
    return user_count;
}

int user_load_from_file(const char *path) {
    int loaded = storage_load_users(users, MAX_USERS, path ? path : "data/users.txt");
    if (loaded < 0) {
        return -1;
    }
    user_count = loaded;
    return loaded;
}

int user_save_to_file(const char *path) {
    return storage_save_users(users, (size_t)user_count, path ? path : "data/users.txt");
}

int user_add(const User *u) {
    if (!u) {
        return -1;
    }

    if (user_count >= MAX_USERS) {
        return -2;
    }

    if (u->name[0] == '\0' || u->course[0] == '\0') {
        return -3;
    }

    for (int i = 0; i < user_count; ++i) {
        if (users[i].id == u->id) {
            return -4;
        }
    }

    users[user_count] = *u;
    user_count++;
    return 0;
}

User *user_find_by_id(int id) {
    for (int i = 0; i < user_count; ++i) {
        if (users[i].id == id) {
            return &users[i];
        }
    }
    return NULL;
}

User *user_find_by_name(const char *name) {
    if (!name) {
        return NULL;
    }

    for (int i = 0; i < user_count; ++i) {
        if (strstr(users[i].name, name) != NULL) {
            return &users[i];
        }
    }
    return NULL;
}

void user_list(void) {
    if (user_count == 0) {
        puts("Nenhum usuario cadastrado.");
        return;
    }

    puts("Usuarios cadastrados:");
    for (int i = 0; i < user_count; ++i) {
        printf("%d) id=%d nome=%s curso=%s\n",
               i + 1,
               users[i].id,
               users[i].name,
               users[i].course);
    }
}

static void user_clear_input(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

void user_run(void) {
    user_init();
    if (user_load_from_file("data/users.txt") < 0) {
        puts("Arquivo de usuarios nao encontrado. Continuando com lista vazia.");
    }

    int option = 0;
    do {
        puts("\n=== Usuarios ===");
        puts("1. Listar usuarios");
        puts("2. Cadastrar usuario");
        puts("3. Buscar usuario por ID");
        puts("4. Buscar usuario por nome");
        puts("0. Voltar");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &option) != 1) {
            user_clear_input();
            option = -1;
            continue;
        }
        user_clear_input();

        switch (option) {
            case 1:
                user_list();
                break;

            case 2: {
                User u;
                char buffer[USER_NAME_LEN];
                char course[USER_COURSE_LEN];

                printf("ID: ");
                if (scanf("%d", &u.id) != 1) {
                    user_clear_input();
                    puts("ID invalido.");
                    break;
                }
                user_clear_input();

                printf("Nome: ");
                if (!fgets(buffer, sizeof(buffer), stdin)) {
                    puts("Nome invalido.");
                    break;
                }
                buffer[strcspn(buffer, "\r\n")] = '\0';
                snprintf(u.name, sizeof(u.name), "%s", buffer);

                printf("Curso: ");
                if (!fgets(course, sizeof(course), stdin)) {
                    puts("Curso invalido.");
                    break;
                }
                course[strcspn(course, "\r\n")] = '\0';
                snprintf(u.course, sizeof(u.course), "%s", course);

                int status = user_add(&u);
                if (status == 0) {
                    user_save_to_file("data/users.txt");
                    puts("Usuario cadastrado com sucesso.");
                } else {
                    printf("Erro ao cadastrar usuario: %d\n", status);
                }
                break;
            }

            case 3: {
                int id;
                printf("Informe o ID: ");
                if (scanf("%d", &id) != 1) {
                    user_clear_input();
                    puts("ID invalido.");
                    break;
                }
                user_clear_input();

                User *found = user_find_by_id(id);
                if (!found) {
                    puts("Usuario nao encontrado.");
                } else {
                    printf("Usuario encontrado: id=%d nome=%s curso=%s\n",
                           found->id,
                           found->name,
                           found->course);
                }
                break;
            }

            case 4: {
                char name[USER_NAME_LEN];
                printf("Informe o nome ou parte do nome: ");
                if (!fgets(name, sizeof(name), stdin)) {
                    puts("Nome invalido.");
                    break;
                }
                name[strcspn(name, "\r\n")] = '\0';

                User *found = user_find_by_name(name);
                if (!found) {
                    puts("Usuario nao encontrado.");
                } else {
                    printf("Usuario encontrado: id=%d nome=%s curso=%s\n",
                           found->id,
                           found->name,
                           found->course);
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
