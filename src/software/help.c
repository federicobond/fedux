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

    printf("chat    [--version]             initiates a chat session by serial port\n");
    printf("cowsay  [-bgpstwy] [--version]  prints a cute cow with the given message\n");
    printf("echo    [-n] [--version]        prints the arguments to stdout\n");
    printf("fortune [--version]             print a fortune cookie\n");
    printf("help    [--version]             print this message\n");
    printf("laws    [--version]             print the three laws of robotics\n");

    return 0;
}

