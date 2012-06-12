#include "../include/syscall.h"
#include "../include/stdlib.h"
#include "../include/stdio.h"
#include "../include/ctype.h"
#include "../include/string.h"
#include "../include/kasm.h"
#include <stdarg.h>

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
    char ch;
    if (stream->lastc)
    {
        ch = stream->lastc;
        stream->lastc = 0;
        return (int)ch;
    }
    while (!read(stream->fd, (void *)&ch, 1))
        _hlt();
    return (int)ch;
}

void
ungetc(int ch, FILE *stream)
{
    stream->lastc = ch;
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
    char c;
    int i = 0; 

    while (i + 1 < size && (c = getc(stream)) != '\n' && c != EOF)
        s[i++] = c;

    s[i] = 0;
    return (i != 0) ? s : NULL;
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
printf_u(unsigned int i, FILE *f)
{
    char buf[32];
    utoa(i, buf, 10);
    return printf_s(buf, f);
}

int
printf_x(unsigned int i, FILE *f)
{
    char buf[32];
    utoa(i, buf, 16);
    strtolower(buf);
    return printf_s(buf, f);
}

int
printf_X(unsigned int i, FILE *f)
{
    char buf[32];
    utoa(i, buf, 16);
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
                case 'u':
                    acum += printf_u(va_arg(ap, unsigned int), f);
                    break;
                case 'x':
                    acum += printf_x(va_arg(ap, unsigned int), f);
                    break;
                case 'X':
                    acum += printf_X(va_arg(ap, unsigned int), f);
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
    retval = vscanf(fmt, ap);
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
vscanf(const char *fmt, va_list ap)
{
    int ch = 0;
    void *ptr;
    int matches = 0;
    while ((ch = getchar()) != '\n')
    {
        if (*fmt == '%')
        {
            fmt++;
            switch (*fmt)
            {
            case 'd':
                ptr = va_arg(ap, int *);
                if (isdigit(ch))
                {
                    matches++;
                    *(int *)ptr = ch - '0';
                }
                while (isdigit(ch = getchar()))
                {
                    *(int *)ptr *= 10;
                    *(int *)ptr += ch - '0';
                }
                if (ch != '\n')
                    ungetc(ch, stdin);
                break;
            case 's':
                ptr = va_arg(ap, char *);
                if (!isspace(ch))
                {
                    matches++;
                    *(char *)ptr++ = ch;
                }
                while (!isspace(ch = getchar()))
                {
                    *(char *)ptr++ = ch;
                }
                if (ch != '\n')
                    ungetc(ch, stdin);
                break;
            default:
                /* Error: invalid format specifier */
                return -1;
            }
        }
        else if (ch != *fmt)
        {
            /* Error: stream does not match format */
            return matches;
        }
        fmt++;
    }
    return matches;
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
