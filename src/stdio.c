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
    /* TODO: This should block while it cannot write, but there isn't anyone to
     * unblock it, so we leave it as is */
    if (write(stream->fd, (void *)&ch, 1) == -1)
        return EOF;
    return ch;
}

int
getc(FILE *stream)
{
    /* TODO: Handle -1 error value */
    char ch;
    while (!read(stream->fd, (void *)&ch, 1))
        _hlt();
    return (int)ch;
}

int
puts(const char *str)
{
    int res;
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
    /* TODO: Return NULL when getc fails */
    char c;
    int i = 0; 

    while ((c = getchar()) != '\n' && c != EOF)
        s[i++] = c;

    s[i] = 0;
    return s;
    /* return (i != 0) ? s : NULL; */
}

int
printf(const char *fmt, ...)
{
    va_list ap;
    int retval;

    va_start(ap, fmt);
    return vfprintf(stdout, fmt, ap);
    va_end(ap);

    return retval;
}

int
sprintf(char *s, const char *fmt, ...)
{
    va_list ap;
    int retval;

    va_start(ap, fmt);
    retval = vsprintf(s, fmt, ap);
    va_end(ap);

    return retval;
}

int
fprintf(FILE *f, const char *fmt, ...)
{
    va_list ap;
    int retval;

    va_start(ap, fmt);
    retval = vfprintf(f, fmt, ap);
    va_end(ap);

    return retval;
}

int
printf_s(char *str, FILE *f)
{
    int i = 0;
    while (str[i] != '\0')
    {
        putc(str[i++], f);
    }
    return i;
}

int
printf_d(int i, FILE *f)
{
    char buf[32];
    itoa(i, buf, 10);
    return printf_s(buf, f);
}

int
vsprintf(char *str, const char *fmt, va_list ap)
{
    /* TODO: Code */
    return 0;
}

int
vfprintf(FILE *f, const char *fmt, va_list ap)
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
                    acum += printf_s(va_arg(ap, char *), f);
                    break;
                case 'd':
                    acum += printf_d(va_arg(ap, int), f);
                    break;
                case 'c':
                    putc(va_arg(ap, int), f);
                    acum++;
                    break;
                case '%':
                    putc('%', f);
                    acum++;
                    break;
                default:
                    return -1;
            }
        }
        else
        {
            putc(fmt[i], f);
            acum++;
        }
        i++;
    }
    return acum;
}

int
scanf(const char *fmt, ...)
{
    va_list ap;
    int retval;

    va_start(ap, fmt);
    retval = vfscanf(stdin, fmt, ap);
    va_end(ap);

    return retval;
}

int
fscanf(FILE *f, const char *fmt, ...)
{
    va_list ap;
    int retval;

    va_start(ap, fmt);
    retval = vfscanf(f, fmt, ap);
    va_end(ap);

    return retval;
}

int
sscanf(char *str, const char *fmt, ...) {
    va_list ap;
    int retval;

    va_start(ap, fmt);
    retval = vsscanf(str, fmt, ap);
    va_end(ap);

    return retval;
}

int
vfscanf(FILE *f, const char *fmt, va_list ap)
{
    /* TODO: Code */
    return 0;
}

int
vsscanf(char *str, const char *fmt, va_list ap)
{
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
