#include "../include/stdio.h"
#include "../include/ctype.h"
#include "../include/string.h"
#include "../include/sh.h"

#define PROMPT "fedux # "

void
sh_init(void)
{
    sh_show_prompt();
}

void
sh_show_prompt()
{
    int i = 0;
    char datum = 0;
    char buf[255];

    while (1)
    {
        printf("%s", PROMPT);
        datum = 0;

        while (datum != '\n')
        {
            if (read(STDIN_FILENO, (void *)&datum, 1))
                 buf[i++] = datum;
        }

        buf[i] = 0;
        buf_trim(buf);

        if (strlen(buf) > 0)
            sh_do_command(buf);

        i = 0;
    }
}

void
sh_read_command(char buf[])
{
    /* TODO: Code */
    return;
}

int
sh_do_command(char buf[])
{
    /* Do argument parsing */
    int argc = 0;
    char *argv[255];

    sh_tokenize(buf, &argc, argv);

    return exec(argc, argv);
}

int
exec(int argc, char **argv)
{
    if (strcmp(argv[0], "laws") == 0)
        return exec_laws(argc, (char **)argv);
    if (strcmp(argv[0], "fortune") == 0)
        return exec_fortune(argc, (char **)argv);
    if (strcmp(argv[0], "echo") == 0)
        return exec_echo(argc, (char **)argv);

    printf("%s: command not found\n", argv[0]);
    return 127;
}

void
sh_tokenize(char buf[], int *argc, char *argv[])
{
    char ch;
    enum state { SPACE, TOKEN, QUOTED_START, QUOTED } state = SPACE;

    while ((ch = *buf) != '\0')
    {
        switch (state)
        {
            case SPACE:
                if (ch == '"')
                {
                    state = QUOTED_START;
                }
                else if (!isspace(ch))
                {
                    state = TOKEN;
                    argv[(*argc)++] = buf;
                }
                break;
            case TOKEN:
                if (isspace(ch))
                {
                    state = SPACE;
                    *buf = '\0';
                }
                break;
            case QUOTED_START:
                argv[(*argc)++] = buf;
                if (ch == '"')
                    state = SPACE;
                else
                    state = QUOTED;
                break;
            case QUOTED:
                if (ch == '"')
                {
                    state = SPACE;
                    *buf = '\0';
                }
                break;
        }
        buf++;
    }
}

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

int
args_shift(int n, int *argc, char ***argv)
{
    if (*argc >= n)
    {
        *argc -= n;
        *argv += n;
    }
}

void
buf_trim(char buf[])
{
    int i, j;
    i = j = 0;
    while (isspace(buf[i]))
    {
        i++;
    }
    while (buf[i] != '\0')
    {
        buf[j++] = buf[i++];
    }    
    buf[j] = '\0';
}
