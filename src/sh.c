#include "../include/stdio.h"
#include "../include/ctype.h"
#include "../include/string.h"
#include "../include/sh.h"

#define PROMPT " > "

void
sh_init(void)
{
    sh_show_prompt();
}

void
sh_show_prompt()
{
    char buf[255];
    while (1)
    {
        printf(PROMPT);
        sh_read_command(buf);
        if (strlen(buf) != 0)
            sh_do_command(buf);
        else
            putchar('\n');
    }
}

void
sh_read_command(char buf[])
{
    char c;
    int i = 0;
    while (i < 255 && (c = getchar()) != '\n')
    {
        buf[i++] = c;
    }
    buf[i] = '\0';
    buf_trim(buf);
}

int
sh_do_command(char buf[])
{
    /* Do argument parsing */
    int argc = 0;
    char *argv[255];

    sh_tokenize(buf, &argc, argv);

    if (strcmp(argv[0], "laws"))
        return exec_laws(argc, (char **)argv);
    if (strcmp(argv[0], "fortune"))
        return exec_fortune(argc, (char **)argv);
    if (strcmp(argv[0], "echo"))
        return exec_fortune(argc, (char **)argv);

    printf("%s: command not found", argv[0]);
    return 0;
}

void
sh_tokenize(char buf[], int *argc, char *argv[])
{
    char ch;
    enum state { SPACE, TOKEN } state = SPACE;

    while ((ch = *buf) != '\0')
    {
        switch (state)
        {
            case SPACE:
                if (ch == '"')
                {
                    state = TOKEN;
                }
                else if (!isspace(ch))
                {
                    state = TOKEN;
                    argv[(*argc)++] = buf;
                }
                break;
            case TOKEN:
                if (ch == '"' || isspace(ch))
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
    printf("  1. A robot may not injure a human being or, through inaction, allow a human being to come to harm.\n");
    printf("  2. A robot must obey the orders given to it by human beings, except where such orders would conflict with the First Law.\n");
    printf("  3. A robot must protect its own existence as long as such protection does not conflict with the First or Second Laws.\n");

    return 0;
}

int
exec_fortune(int argc, char **argv)
{
    printf("No fortune cookie for you today\n");
    
    return 0;
}

int
exec_echo(int argc, char **argv)
{
    while (--argc)
    {
        printf("%s ", *++argv);
    }
    putchar('\n');

    return 0;
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
