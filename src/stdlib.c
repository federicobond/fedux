#include "../include/stdlib.h"
#include "../include/string.h"
#include "../include/errno.h"
#include "../include/ctype.h"

int errno;
unsigned int rand_seed = 4;

int
rand()
{
    /* Chosen by fair dice roll. Guaranteed to be random */
    return rand_seed;
}

void
srand(unsigned int seed)
{
    rand_seed = seed;
}

int
abs(int number)
{
    return (number < 0) ? -number : number;
}


long
labs(long number)
{
    return (number < 0) ? -number : number;
}

div_t
div(int numerator, int denominator)
{
    /* TODO: Handle 0 as denominator. Check POSIX standard */
    /* TODO: Handle negative numbers.
     * Remember that result must satify quot * denominator + rem = numerator */
    div_t result = {0, 0};
    result.quot = numerator / denominator;
    result.rem = numerator % denominator;

    return result;
}

int
atoi(const char *str)
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

int
itoa(int value, char *s, int radix)
{
    char *buf = s;
    int rem, len;
    char tmp;

    /* Only supports 10 radix. Parameter is for maintaining a compatible api */
    radix = 10;

    if (value < 0)
    {
        *buf++ = '-';
        value = -value;
        s++;
    }
    do
    {
        rem = value % radix;
        value = value / radix;

        *buf++ = rem + '0';
    }
    while (value > 0);

    *buf = 0;
    len = buf - s;

    buf--;
    while (buf > s)
    {
        tmp = *buf;
        *buf = *s;
        *s = tmp;

        s++;
        buf--;
    }
    return len;
}
