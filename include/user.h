#ifndef USER_H
#define USER_H

#define USER_NAME_LEN 128
#define USER_COURSE_LEN 128

void user_init(void);
int user_add(const struct User *u);
struct User *user_find_by_id(int id);
struct User *user_find_by_name(const char *name);
void user_list(void);
void user_run(void);

typedef struct User {
    int id;
    char name[USER_NAME_LEN];
    char course[USER_COURSE_LEN];
} User;

#endif 