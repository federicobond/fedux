#ifndef _STDIO_H
#define _STDIO_H

#include "stddef.h"

#ifndef NULL
#define NULL 0
#endif

/* Standard file descriptors. */
#define STDIN_FILENO    0
#define STDOUT_FILENO   1
#define STDERR_FILENO   2

typedef struct {
    int fd;
} FILE;

// FILE *stdin  = { 0 };
// FILE *stdout = { 0 };
// FILE *stderr = { 0 };

int     putc(int ch, FILE *stream);
int     getc(FILE *stream);

int     putchar(int ch);
int     getchar(void);
int     ungetchar(int ch);

char   *gets(char *str);
int     puts(const char *str);

int     printf(const char* fmt, ...);
int     scanf(const char *fmt, ...);
int     sscanf(char *buf, const char *fmt, ...);

ssize_t write(int fd, const void *buf, size_t count);
ssize_t read(int fd, void *buf, size_t count);

#endif
