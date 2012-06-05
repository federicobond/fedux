#ifndef _STDIO_H
#define _STDIO_H

#include "stddef.h"
#include <stdarg.h>

#ifndef EOF
#define EOF (-1)
#endif

/* Standard file descriptors. */
#define STDIN_FILENO    0
#define STDOUT_FILENO   1
#define STDERR_FILENO   2

typedef struct {
    int fd;
} FILE;

extern FILE *stdin, *stdout, *stderr;

int     putc(int ch, FILE *stream);
int     getc(FILE *stream);

int     putchar(int ch);
int     getchar(void);
int     ungetchar(int ch);

/* DEPRECATED */
/* char   *gets(char *str); */

int     puts(const char *str);

char   *fgets(char *s, int size, FILE *stream);

int     printf(const char *fmt, ...);
int     vprintf(const char *fmt, va_list ap);
int     scanf(const char *fmt, ...);
int     sscanf(char *buf, const char *fmt, ...);

ssize_t write(int fd, const void *buf, size_t count);
ssize_t read(int fd, void *buf, size_t count);

#endif
