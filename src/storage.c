#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/storage.h"

#define STORAGE_LINE_LEN 512

static char *storage_trim(char *text) {
    if (!text) return NULL;

    while (*text == ' ' || *text == '\t' || *text == '\r' || *text == '\n') {
        text++;
    }

    if (*text == '\0') return text;

    char *end = text + strlen(text) - 1;
    while (end > text && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n')) {
        *end = '\0';
        end--;
    }

    return text;
}

static int storage_parse_int(const char *text, int *value) {
    char *end = NULL;
    long parsed = strtol(text, &end, 10);
    if (text == end || *text == '\0') {
        return -1;
    }
    *value = (int)parsed;
    return 0;
}

static int storage_parse_bool(const char *text, bool *value) {
    if (!text || !value) return -1;

    if (strcmp(text, "1") == 0 || strcmp(text, "true") == 0 || strcmp(text, "TRUE") == 0) {
        *value = true;
        return 0;
    }

    if (strcmp(text, "0") == 0 || strcmp(text, "false") == 0 || strcmp(text, "FALSE") == 0) {
        *value = false;
        return 0;
    }

    return -1;
}

int storage_load_users(User users[], size_t max_users, const char *path) {
    FILE *file = fopen(path, "r");
    if (!file) {
        return -1;
    }

    char line[STORAGE_LINE_LEN];
    int count = 0;

    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        return 0;
    }

    while (fgets(line, sizeof(line), file)) {
        char *record = storage_trim(line);
        if (*record == '\0' || *record == '#') {
            continue;
        }

        if (count >= (int)max_users) {
            break;
        }

        char *fields[3] = {0};
        int field_count = 0;
        char *token = strtok(record, ";");
        while (token && field_count < 3) {
            fields[field_count++] = storage_trim(token);
            token = strtok(NULL, ";");
        }

        if (field_count < 3) {
            continue;
        }

        if (storage_parse_int(fields[0], &users[count].id) != 0) {
            continue;
        }

        snprintf(users[count].name, sizeof(users[count].name), "%s", fields[1]);
        snprintf(users[count].course, sizeof(users[count].course), "%s", fields[2]);
        count++;
    }

    fclose(file);
    return count;
}

int storage_save_users(const User users[], size_t count, const char *path) {
    FILE *file = fopen(path, "w");
    if (!file) {
        return -1;
    }

    fprintf(file, "id;name;course\n");
    for (size_t i = 0; i < count; ++i) {
        fprintf(file, "%d;%s;%s\n",
                users[i].id,
                users[i].name,
                users[i].course);
    }

    fclose(file);
    return 0;
}

int storage_load_books(Book books[], size_t max_books, const char *path) {
    FILE *file = fopen(path, "r");
    if (!file) {
        return -1;
    }

    char line[STORAGE_LINE_LEN];
    int count = 0;

    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        return 0;
    }

    while (fgets(line, sizeof(line), file)) {
        char *record = storage_trim(line);
        if (*record == '\0' || *record == '#') {
            continue;
        }

        if (count >= (int)max_books) {
            break;
        }

        char *fields[5] = {0};
        int field_count = 0;
        char *token = strtok(record, ";");
        while (token && field_count < 5) {
            fields[field_count++] = storage_trim(token);
            token = strtok(NULL, ";");
        }

        if (field_count < 5) {
            continue;
        }

        if (storage_parse_int(fields[0], &books[count].id) != 0) {
            continue;
        }

        snprintf(books[count].title, sizeof(books[count].title), "%s", fields[1]);
        snprintf(books[count].author, sizeof(books[count].author), "%s", fields[2]);
        if (storage_parse_int(fields[3], &books[count].year) != 0) {
            continue;
        }
        if (storage_parse_int(fields[4], &books[count].quantity) != 0) {
            continue;
        }

        count++;
    }

    fclose(file);
    return count;
}

int storage_save_books(const Book books[], size_t count, const char *path) {
    FILE *file = fopen(path, "w");
    if (!file) {
        return -1;
    }

    fprintf(file, "id;title;author;year;quantity\n");
    for (size_t i = 0; i < count; ++i) {
        fprintf(file, "%d;%s;%s;%d;%d\n",
                books[i].id,
                books[i].title,
                books[i].author,
                books[i].year,
                books[i].quantity);
    }

    fclose(file);
    return 0;
}

int storage_load_loans(Loan loans[], size_t max_loans, const char *path) {
    FILE *file = fopen(path, "r");
    if (!file) {
        return -1;
    }

    char line[STORAGE_LINE_LEN];
    int count = 0;

    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        return 0;
    }

    while (fgets(line, sizeof(line), file)) {
        char *record = storage_trim(line);
        if (*record == '\0' || *record == '#') {
            continue;
        }

        if (count >= (int)max_loans) {
            break;
        }

        char *fields[5] = {0};
        int field_count = 0;
        char *token = strtok(record, ";");
        while (token && field_count < 5) {
            fields[field_count++] = storage_trim(token);
            token = strtok(NULL, ";");
        }

        if (field_count < 5) {
            continue;
        }

        if (storage_parse_int(fields[0], &loans[count].book_id) != 0) {
            continue;
        }
        if (storage_parse_int(fields[1], &loans[count].user_id) != 0) {
            continue;
        }

        snprintf(loans[count].date_loan, sizeof(loans[count].date_loan), "%s", fields[2]);
        snprintf(loans[count].date_return, sizeof(loans[count].date_return), "%s", fields[3]);

        if (storage_parse_bool(fields[4], &loans[count].returned) != 0) {
            loans[count].returned = false;
        }

        count++;
    }

    fclose(file);
    return count;
}

int storage_save_loans(const Loan loans[], size_t count, const char *path) {
    FILE *file = fopen(path, "w");
    if (!file) {
        return -1;
    }

    fprintf(file, "book_id;user_id;date_loan;date_return;returned\n");
    for (size_t i = 0; i < count; ++i) {
        fprintf(file, "%d;%d;%s;%s;%s\n",
                loans[i].book_id,
                loans[i].user_id,
                loans[i].date_loan,
                loans[i].date_return,
                loans[i].returned ? "true" : "false");
    }

    fclose(file);
    return 0;
}
