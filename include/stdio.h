#ifndef _STDIO_H
#define _STDIO_H

#define NULL 0

/* Standard file descriptors. */
#define STDIN_FILENO    0
#define STDOUT_FILENO   1
#define STDERR_FILENO   2

FILE *stdin  = {STDIN_FILENO};
FILE *stdout = {STDOUT_FILENO};
FILE *stderr = {STDERR_FILENO};

typedef struct {
    int fd;
} FILE;

typedef unsigned int size_t;
typedef int ssize_t;

int     putc(int ch, FILE *stream);
int     getc(FILE *stream);

int     putchar(int ch);
int     getchar(void);
int     ungetchar(int ch);

char   *gets(char *str);

int     printf(const char* fmt, ...);
int     scanf(const char *fmt, ...);
int     sscanf(char *buf, const char *fmt, ...);

ssize_t write(int fd, const void *buf, size_t count);
ssize_t read(int fd, void *buf, size_t count);

#endif
