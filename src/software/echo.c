#include "../../include/stdio.h"
#include "../../include/string.h"
#include "../../include/software/commons.h"

int
exec_echo(int argc, char **argv)
{
    bool nl = true;

    args_shift(1, &argc, &argv);

    if (argc == 1 && strcmp(argv[0], "--version") == 0)
    {
        printf("echo 1.0 (Fedux release 1.0)\n");
        printf("Copyright (C) 2012 Federico Pomar & Federico Bond\n");
        return 0;
    }

    if (argc > 0 && strcmp(argv[0], "-n") == 0)
    {
        nl = false;
        args_shift(1, &argc, &argv);
    }

    while (argc)
    {
        printf("%s", *argv++);
        if (--argc != 0)
            putchar(' ');
    }

    if (nl)
        putchar('\n');

    return 0;
}

