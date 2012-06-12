#include "../../include/stdio.h"
#include "../../include/stdlib.h"
#include "../../include/time.h"
#include "../../include/string.h"
#include "../../include/software/commons.h"

#define COOKIE_COUNT 6

static char *cookies[] = {
    "Any intelligent fool can make things bigger and more complex... It takes a touch of genius - and a lot of courage to move in the opposite direction\n    - Albert Einstein",
    "Art is never finished, only abandoned.\n    - Leonardo Da Vinci",
    "A creative man is motivated by the desire to achieve, not by the desire to beat others.\n    - Ayn Rand",
    "Any sufficiently advanced technology is indistinguishable from magic.\n    - Arthur C. Clarke",
    "There are only two hard things in Computer Science: cache invalidation and naming things\n    - Phil Karlton",
    "Poets say science takes away from the beauty of the stars - mere globs of gas atoms. I, too, can see the stars on a desert night, and feel them. But do I see less or more?\n    - Richard P. Feynman"
};

int
exec_fortune(int argc, char **argv)
{
    args_shift(1, &argc, &argv);

    if (argc == 1 && strcmp(argv[0], "--version") == 0)
    {
        printf("fortune 1.0 (Fedux release 1.0)\n");
        printf("Copyright (C) 2012 Federico Pomar & Federico Bond\n");
        return 0;
    }

    srand(time(NULL));
    printf("%s\n", cookies[rand() % COOKIE_COUNT]);
    
    return 0;
}

