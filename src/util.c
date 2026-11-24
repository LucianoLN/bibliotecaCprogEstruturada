#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

void str_copy(char *dst, const char *src, size_t size) {
    snprintf(dst, size, "%s", src ? src : "");
}

int str_icmp(const char *a, const char *b) {
    while (*a && tolower((unsigned char)*a) == tolower((unsigned char)*b)) {
        a++;
        b++;
    }
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}

int str_icontains(const char *text, const char *term) {
    size_t n = strlen(term);
    for (; *text; text++) {
        size_t i = 0;
        while (i < n && text[i] &&
               tolower((unsigned char)text[i]) == tolower((unsigned char)term[i]))
            i++;
        if (i == n) return 1;
    }
    return n == 0;
}

int parse_int(const char *s, int *out) {
    char *end;
    long long v;
    errno = 0;
    v = strtoll(s, &end, 10);
    while (isspace((unsigned char)*end)) end++;
    if (end == s || *end != '\0' || errno == ERANGE || v < INT_MIN || v > INT_MAX)
        return 0;
    *out = (int)v;
    return 1;
}

time_t date_make(int day, int month, int year) {
    struct tm tm;
    time_t t;
    memset(&tm, 0, sizeof tm);
    tm.tm_mday = day;
    tm.tm_mon = month - 1;
    tm.tm_year = year - 1900;
    tm.tm_hour = 12;              
    tm.tm_isdst = -1;
    t = mktime(&tm);
    if (t == (time_t)-1 || tm.tm_mday != day || tm.tm_mon != month - 1 ||
        tm.tm_year != year - 1900)
        return (time_t)-1;
    return t;
}

time_t date_today(void) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    return t ? date_make(t->tm_mday, t->tm_mon + 1, t->tm_year + 1900) : now;
}

int date_to_ymd(time_t t) {
    struct tm *tm = t ? localtime(&t) : NULL;
    return tm ? (tm->tm_year + 1900) * 10000 + (tm->tm_mon + 1) * 100 + tm->tm_mday : 0;
}

time_t date_from_ymd(int ymd) {
    return ymd ? date_make(ymd % 100, ymd / 100 % 100, ymd / 10000) : 0;
}

void date_format(time_t t, char *buf, size_t size) {
    struct tm *tm = t ? localtime(&t) : NULL;
    if (!tm || strftime(buf, size, "%d/%m/%Y", tm) == 0) str_copy(buf, "-", size);
}

#define SECONDS_PER_DAY 86400

int date_parse(const char *s, time_t *out) {
    int day, month, year;
    char extra;
    time_t t;
    if (sscanf(s, "%d/%d/%d %c", &day, &month, &year, &extra) != 3) return 0;
    t = date_make(day, month, year);
    if (t == (time_t)-1) return 0;
    *out = t;
    return 1;
}

time_t date_add_days(time_t t, int days) {
    return t + (time_t)days * SECONDS_PER_DAY;
}

int date_diff_days(time_t from, time_t to) {
    double days = difftime(to, from) / SECONDS_PER_DAY;
    return (int)(days < 0 ? days - 0.5 : days + 0.5);
}
