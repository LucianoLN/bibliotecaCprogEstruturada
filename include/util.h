#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>
#include <time.h>

void str_copy(char *dst, const char *src, size_t size);

int str_icmp(const char *a, const char *b);

int str_icontains(const char *text, const char *term);

int parse_int(const char *s, int *out);

time_t date_make(int day, int month, int year);
time_t date_today(void);

int date_to_ymd(time_t t);
time_t date_from_ymd(int ymd);

void date_format(time_t t, char *buf, size_t size);

int date_parse(const char *s, time_t *out);
time_t date_add_days(time_t t, int days);
int date_diff_days(time_t from, time_t to);

#endif
