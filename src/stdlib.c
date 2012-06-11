#include "../include/stdlib.h"
#include "../include/string.h"
#include "../include/errno.h"
#include "../include/ctype.h"

int errno;
/* Chosen by fair dice roll. Guaranteed to be rand */
unsigned long rand_seed = 4;

int
rand(void)
{
    /* Blatant ripoff from K&R */
    rand_seed = rand_seed * 1103515245 + 12345;
    return (unsigned int)(rand_seed / 65536) % 32768;
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

char *
itoa(int value, char *s, int base)
{
	int numeral, i = 0;
    char *sb = s;

    if (base == 10 && value < 0)
    {
        *s++ = '-';
        value = -value;
    }

	if (base >= 2 && base <= 36)
	{
		do
		{
			numeral = value % base;
			value = value/base;
			if (numeral >= 0 && numeral <= 9)		
				s[i] = '0' + numeral;
			else
				s[i] = 'A' + numeral - 10;
			i++;
		}
		while (value);
	}

	s[i] = '\0';

    strrev(s);
	
    return sb;
}

char *
utoa(unsigned int value, char *s, int base)
{
	unsigned int numeral, i = 0;
    char *sb = s;

	if (base >= 2 && base <= 36)
	{
		do
		{
			numeral = value % base;
			value = value/base;
			if (numeral >= 0 && numeral <= 9)
				s[i] = '0' + numeral;
			else
				s[i] = 'A' + numeral - 10;
			i++;
		}
		while (value);
	}

	s[i] = '\0';

    strrev(s);

    return sb;
}
