#ifndef PERSIST_H
#define PERSIST_H

#include "storage.h"

#define PERSIST_ERROR   (-1)
#define PERSIST_NO_FILE (-2)
#define BIN_FILE        "biblioteca.bin"

int persist_save_bin(const Library *lib, const char *path);

int persist_load_bin(Library *lib, const char *path);

int persist_save_txt(const Library *lib, const char *prefix);
int persist_load_txt(Library *lib, const char *prefix);

#endif 
