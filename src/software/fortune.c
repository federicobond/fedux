#include "../../include/stdio.h"
#include "../../include/string.h"
#include "../../include/software/commons.h"

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

    printf("No fortune cookie for you today\n");
    
    return 0;
}

