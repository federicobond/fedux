#include "../include/stdio.h"

int
putc(int ch, FILE *stream)
{
    char buf[1];
    buf[0] = ch;
    write(stream->fd, (void *)buf, 1);
}

int
getc(FILE *stream)
{
    return 0;
}

int putchar(int ch)
{
    putc(ch, stdout);
}

int getchar(void)
{
    getc(stdin);
}

ssize_t
read(int fd, void *buffer, size_t size)
{
    return 0;
}

ssize_t
write(int fd, const void *buffer, size_t size)
{
    return 0;
}
