#include "../../include/stdio.h"
#include "../../include/software/commons.h"

int
args_shift(int n, int *argc, char ***argv)
{
    if (*argc >= n)
    {
        *argc -= n;
        *argv += n;
    }
    return *argc - n;
}

