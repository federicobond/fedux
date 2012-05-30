#include "../include/ctype.h"

int isdigit(int c)
{
    return c >= '0' && c <= '9';
}

int isupper(int c)
{
    return c >= 'A' && c <= 'Z';
}

int islower(int c)
{
    return c >= 'a' && c <= 'Z';
}

int toupper(int c)
{
    return (islower(c)) ? c - 32 : c;
}

int tolower(int c)
{
    return (isupper(c)) ? c + 32 : c;
}

int isalpha(int c)
{
    return isupper(c) || islower(c);
}

int isalnum(int c)
{
    return isalpha(c) || isdigit(c);
}

int isascii(int c)
{
    return c >= 0 && c <= 127;
}

int iscntrl(int c)
{
    return c == 0x7F || (c >= 0x00 && c <= 0x1F);
}

int isspace(int c)
{
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}
