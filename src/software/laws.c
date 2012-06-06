#include "../../include/stdio.h"
#include "../../include/string.h"
#include "../../include/software/commons.h"

int
exec_laws(int argc, char **argv)
{
    args_shift(1, &argc, &argv);

    if (argc == 1 && strcmp(argv[0], "--version") == 0)
    {
        printf("laws 1.0 (Fedux release 1.0)\n");
        printf("Copyright (C) 2012 Federico Pomar & Federico Bond\n");
        return 0;
    }

    printf("  1. A robot may not injure a human being or, through inaction, allow a human being to come to harm.\n");
    printf("  2. A robot must obey the orders given to it by human beings, except where such orders would conflict with the First Law.\n");
    printf("  3. A robot must protect its own existence as long as such protection does not conflict with the First or Second Laws.\n");

    return 0;
}

