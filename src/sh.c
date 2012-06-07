#include "../include/stdio.h"
#include "../include/ctype.h"
#include "../include/string.h"
#include "../include/sh.h"

int exec_laws(int argc, char **argv);
int exec_fortune(int argc, char **argv);
int exec_echo(int argc, char **argv);
int exec_help(int argc, char **argv);

typedef struct {
    char *name;
    int (*exec)(int argc, char **argv);
} command_t;

command_t commands[] = {
    { "laws",       exec_laws },
    { "fortune",    exec_fortune },
    { "echo",       exec_echo },
    { "help",       exec_help },
    { NULL, NULL }
};

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
    char buf[256];

    while (1)
    {
        printf("%s", PROMPT);
        datum = 0;

        while ((datum = getchar()) != '\n')
                buf[i++] = datum;

        buf[i] = 0;
        trim(buf);

        if (strlen(buf) > 0)
            sh_do_command(buf);

        i = 0;
    }
}

int
sh_do_command(char buf[])
{
    int argc = 0;
    char *argv[256];
    sh_tokenize(buf, &argc, argv);

    return exec(argc, argv);
}

int
exec(int argc, char **argv)
{
    int i = 0;

    while (commands[i].name != NULL)
    {
        if (strcmp(argv[0], commands[i].name) == 0)
            return commands[i].exec(argc, argv);

        i++;
    }

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
