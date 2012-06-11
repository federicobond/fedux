#ifndef _STDLIB_H
#define _STDLIB_H

int     rand(void);
void    srand(unsigned int seed);

int     atof(const char *nptr);
int     atoi(const char *nptr);
long    atol(const char *nptr);

char	*itoa(int value, char *str, int base);
char	*utoa(unsigned int value, char *str, int base);

int     abs(int x);
long    labs(long x);

#endif
