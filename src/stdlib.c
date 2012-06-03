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

char *
itoa(int value, char *s, int base)
{
	int numeral, i = 0;

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
	
    return strrev(s);
}
