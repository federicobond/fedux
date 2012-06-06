#include "../include/syscall.h"
#include "../include/stdlib.h"
#include "../include/stdio.h"
#include "../include/string.h"
#include "../include/kasm.h"
#include <stdarg.h>

/*
#include "../include/kbd.h"
#include "../include/vgatext.h"
*/

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
    /* TODO: Fix */
    int ch;
    if (read(stream->fd, (void *)&ch, 1) == -1)
        return EOF;
    return ch;
}

int
puts(const char *str)
{
    int res;
    char nl = '\n';
    while (*str != '\0')
    {
        res = putchar(*str);
        if (res == -1)
            return EOF;
        str++;
    }
    res = putchar('\n');
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
    /* TODO: Fix */
    return getc(stdin);
}

char *gets(char *s, int size)
{
    return fgets(s, size, stdin);
}


char *
fgets(char *s, int size, FILE *stream)
{
    /* TODO: Fix */
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
printf(const char *fmt, ...)
{
    va_list ap;
    int res;

    va_start(ap, fmt);
    res = vprintf(fmt, ap);
    va_end(ap);

    return res;
}

int
print_string(char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        putchar(str[i++]);
    }
    return i;
}

int
print_int(int i)
{
    char buf[32];
    itoa(i, buf, 10);
    return print_string(buf);
}

int
vprintf(const char *fmt, va_list ap)
{
    int i = 0;
    int acum = 0;

    while (fmt[i] != '\0')
    {
        if (fmt[i] == '%')
        {
            i++;
            switch (fmt[i])
            {
                case 's':
                    acum += print_string(va_arg(ap, char *));
                    break;
                case 'd':
                    acum += print_int(va_arg(ap, int));
                    break;
                case 'c':
                    putchar(va_arg(ap, int));
                    acum++;
                    break;
                case '%':
                    putchar('%');
                    acum++;
                    break;
                default:
                    return -1;
            }
        }
        else
        {
            putchar(fmt[i]);
        }
        acum++;
        i++;
    }
    return acum;
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
    return _syscall(SYSCALL_READ, fd, (int)buf, count, 0, 0);
}

ssize_t
write(int fd, const void *buf, size_t count)
{
    return _syscall(SYSCALL_WRITE, fd, (int)buf, count, 0, 0);
}
