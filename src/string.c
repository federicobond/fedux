#include "../include/string.h"

char *
strcpy(char *dest, const char *src)
{
    char *ret = dest;
    while (*src != '\0')
    {
        *dest++ = *src++; 
    }
    *dest = '\0';
    return ret;
}

char *
strncpy(char *dest, const char *src, size_t n)
{
    char *ret = dest;
    int i = 0;
    while (i < n && *src != '\0')
    {
        *dest++ = *src++; 
    }
    *dest = '\0';
    return ret;
}

size_t
strlen(const char *s)
{
    size_t i = 0;
    while (*s++ != '\0')
    {
        i++;
    }
    return i;
}

int
strcmp(const char *s1, const char *s2)
{
    int i = 0;
    while (s1[i] == s2[i])
    {
        i++;
    }
    return *s2 - *s1;
}

int
strncmp(const char *s1, const char *s2, size_t n)
{
    int i = 0;
    while (i < n && s1[i] == s2[i])
    {
        i++;
    }
    return *s2 - *s1;
}
