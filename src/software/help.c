#include "../../include/stdio.h"
#include "../../include/string.h"
#include "../../include/software/commons.h"

int
exec_help(int argc, char **argv)
{
    args_shift(1, &argc, &argv);

    if (argc == 1 && strcmp(argv[0], "--version") == 0)
    {
        printf("help 1.0 (Fedux release 1.0)\n");
        printf("Copyright (C) 2012 Federico Pomar & Federico Bond\n");
        return 0;
    }

    printf("help    [--version]      print this message\n");
    printf("laws    [--version]      print the three laws of robotics\n");
    printf("fortune [--version]      print a fortune cookie\n");
    printf("echo    [-n] [--version] prints the arguments to stdout\n");

    return 0;
}

