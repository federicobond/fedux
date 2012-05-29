#ifndef _STDLIB_H
#define _STDLIB_H

typedef struct {
    int quot;
    int rem;
} div_t;

typedef struct {
    long quot;
    long rem;
} ldiv_t;

int     atof(const char *nptr);
int     atoi(const char *nptr);
long    atol(const char *nptr);

int     abs(int x);
long    labs(long x);

div_t   div(int numerator, int denominator);
ldiv_t  ldiv(long numerator, long denominator);

#endif
