#ifndef _STRING_H
#define _STRING_H


#ifndef _size_t
typedef int size_t;
#endif

char   *strcpy(char *dest, const char *src);
char   *strncpy(char *dest, const char *src, size_t n);
size_t  strlen(const char *s);
int     strcmp(const char *s1, const char *s2);
int     strncmp(const char *s1, const char *s2, size_t n);

#endif
