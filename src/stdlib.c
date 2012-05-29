#include "../include/errno.h"
#include "../include/ctype.h"

extern int errno;
unsigned int rand_seed = 4;

int rand()
{
    // Chosen by fair dice roll. Guaranteed to be random
    return rand_seed;
}

void srand(unsigned int seed)
{
    rand_seed = seed;
}

int abs(int number)
{
    return (number < 0) ? -number : number;
}


long labs(long number)
{
    return (number < 0) ? -number : number;
}

div_t div(int numerator, int denominator)
{
    div_t result = {0, 0};
    result.quot = numerator / denominator;
    result.rem = numerator % denominator;
}

int atoi(const char *str)
{
    int result = 0;
    while (*str != 0)
    {
        if (!isdigit(*str))
        {
           errno = EINVAL;
           return 0;
        }
        result = result * 10 + (*str - '0');
        str++;
    }
    return result;
}

