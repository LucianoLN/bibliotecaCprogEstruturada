#include <stdio.h>

#include "cli.h"
#include "persist.h"
#include "storage.h"

int main(void) {
    Library lib;
    int rc;

    lib_init(&lib);
    rc = persist_load_bin(&lib, BIN_FILE);
    if (rc == PERSIST_NO_FILE) {
        rc = persist_load_txt(&lib, "");
        if (rc == PERSIST_NO_FILE) {
            puts("Nenhum dado encontrado. Iniciando biblioteca vazia.");
        } else if (rc == PERSIST_ERROR) {
            fputs("Erro ao carregar os arquivos de dados.\n", stderr);
            lib_free(&lib);
            return 1;
        } else {
            printf("%d registro(s) carregado(s) dos arquivos de texto.\n", rc);
        }
    } else if (rc == PERSIST_ERROR) {
        fprintf(stderr, "Erro ao carregar %s; os dados nao foram alterados.\n", BIN_FILE);
        lib_free(&lib);
        return 1;
    } else {
        printf("%d registro(s) carregado(s) de %s.\n", rc, BIN_FILE);
    }

    cli_run(&lib);

    if (persist_save_bin(&lib, BIN_FILE) != 0) {
        fprintf(stderr, "Erro ao salvar %s.\n", BIN_FILE);
        lib_free(&lib);
        return 1;
    }

    lib_free(&lib);
    return 0;
}
