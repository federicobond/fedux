#include "../include/stdio.h"
#include "../include/string.h"

FILE _stdin = { STDIN_FILENO };
FILE _stdout = { STDOUT_FILENO };
FILE _stderr = { STDERR_FILENO };

FILE *stdin = &_stdin;
FILE *stdout = &_stdout;
FILE *stderr = &_stderr;

int
putc(int ch, FILE *stream)
{
    if (write(stream->fd, (void *)&ch, 1) == -1)
        return EOF;

    return ch;
}

int
getc(FILE *stream)
{
    int ch;
    read(stream->fd, &ch, 1);
    return ch;
}

int
puts(const char *str)
{
    int res;
    char nl = '\n';
    while (*str++ != '\0')
    {
        res = write(STDIN_FILENO, (void *)str, 1);
        if (res == -1)
            return EOF;
    }
    res = write(STDIN_FILENO, (void *)&nl, 1);
    if (res == -1)
        return EOF;
    /* Return a non-negative number on success. */
    return 0;
}

int
putchar(int ch)
{
    return putc(ch, stdout);
}

int
getchar(void)
{
    return getc(stdin);
}

char *gets(char *s, int size)
{
    return fgets(s, size, stdin);
}


char *
fgets(char *s, int size, FILE *stream)
{
    char c = getc(stream);
    int i = 0; 
    while ((c != '\n' || c != EOF) && i < size)
    {
        *s++ = c;
        i++;
        c = getc(stream);
    }
    return (i != 0) ? s : NULL;
}

int
printf(const char* fmt, ...)
{
    /* TODO: Code */
    return 0;
}

int
scanf(const char *fmt, ...)
{
    /* TODO: Code */
    return 0;
}

int
sscanf(char *buf, const char *fmt, ...) {
    /* TODO: Code */
    return 0;
}

ssize_t
read(int fd, void *buf, size_t count)
{
    /* TODO: Code */
    return 0;
}

ssize_t
write(int fd, const void *buf, size_t count)
{
    /* TODO: Code */
    return 0;
}
