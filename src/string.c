#include "../include/ctype.h"
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
        i++;
    }
    while (i < n)
    {
        *dest++ = '\0';
        i++;
    }
    return ret;
}


char *
strcat(char *dest, const char *src)
{
	return strcpy(dest + strlen(dest), src);
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
	
    while (s1[i] == s2[i]  && 
		   s1[i] != '\0'	&&
		   s2[i] != '\0')
    {
        i++;
    }
	
	
    return s1[i] - s2[i];
}

int
strncmp(const char *s1, const char *s2, size_t n)
{
    int i = 0;
    while (i < n && s1 == s2)
    {
        i++;
        s1++;
        s2++;
    }
    return *s1 - *s2;
}


char *
strrev(char *str)
{
      char *p1, *p2;

      if (!str || !*str)
            return str;

      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }

      return str;
}

char *
strchr(const char *s, int c)
{
    while (*s != 0)
    {
        if (*s == c)
            return (char *)s;
        s++;
    }
    return NULL;
}

char *
strrchr(const char *s, int c)
{
    char *last = NULL;
    while (*s != 0)
    {
        if (*s == c)
            last = (char *)s;
        s++;
    }
    return last;
}

void *
memcpy(void *dest, const void *src, size_t n)
{
    char *d = (char *)dest;
    char *s = (char *)src;
    while (n--)
    {
        *d++ = *s++;
    }
    return dest;
}

int
memcmp(const void *s1, const void *s2, size_t n)
{
    unsigned char *c1 = (unsigned char *)s1;
    unsigned char *c2 = (unsigned char *)s2;
    while (--n && *c1 == *c2)
    {
        c1++;
        c2++;
    }
    return *c1 - *c2;
}

void *
memset(void *s, int c, size_t n)
{
    unsigned char *p = (unsigned char *)s;
    while (n--)
    {
        *p++ = c;
    }
    return s;
}

void
trim(char *str)
{
    int i = 0, j = 0;
    while (isspace(str[i]))
    {
        i++;
    }
    while (str[i] != '\0')
    {
        str[j++] = str[i++];
    }
    str[j] = '\0';
}
