#include "../include/stdio.h"
#include "../include/string.h"

int
putc(int ch, FILE *stream)
{
    write(stream->fd, (void *)&ch, 1);
}

int
getc(FILE *stream)
{
    return 0;
}

int
puts(const char *str)
{
    write(STDIN_FILENO, (void *)str, strlen(str));
}

int
putchar(int ch)
{
    putc(ch, (FILE *)NULL);
}

int
getchar(void)
{
    getc((FILE *)NULL);
}

char *
gets(char *buf)
{
    /* TODO: Code */
    return NULL;
}

int
printf(const char* fmt, ...)
{
    /* TODO: Implement proper string replacement parameters */
    return puts(fmt);
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
