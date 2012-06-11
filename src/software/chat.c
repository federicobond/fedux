#include "../../include/kasm.h"
#include "../../include/stdio.h"
#include "../../include/string.h"
#include "../../include/ctype.h"
#include "../../include/software/commons.h"
#include "../../include/serialman.h"

#define MAX_NAME 64

void chat_help(char *args);
void chat_name(char *args);
void chat_setname(char *args);
void chat_exit(char *args);
void chat_init(void);

static bool exit = false;

typedef struct {
    char *name;
    void (*exec)(char *args);
} chat_command_t;


static chat_command_t chat_commands[] = {
    { "/help", chat_help },
    { "/exit", chat_exit },
    { "/quit", chat_exit },
    { NULL, NULL }
};

void
chat_help(char *args)
{
    printf("Type /help to show this message again.\n");
    printf("Type /exit or /quit to leave the chat.\n");
}

void
chat_exit(char *args)
{
    printf("Leaving chat.\n");
    exit = true;
}

bool
chat_command(char *s)
{
    int i;
    int length;
    for (i = 0; chat_commands[i].name != NULL; i++)
    {
        length = strlen(chat_commands[i].name);
        if (strncmp(s, chat_commands[i].name, length) == 0
            && isspace(s[length]))
        {
            chat_commands[i].exec(s);
            return true;
        }
    }
    return false;
}

int
exec_chat(int argc, char **argv)
{
    args_shift(1, &argc, &argv);

    if (argc == 1 && strcmp(argv[0], "--version") == 0)
    {
        printf("chat 1.0 (Fedux release 1.0)\n");
        printf("Copyright (C) 2012 Federico Pomar & Federico Bond\n");
        return 0;
    }

    printf("Welcome to fedux-chat!\n");

    chat_help(NULL);
    chat_init();

    return 0;
}

bool
read_from_local(char *buffer, int *idx)
{
    char ch;
    if (read(STDIN_FILENO, &ch, sizeof(char)))
    {
        buffer[(*idx)++] = ch;
        if (ch == '\n')
        {
            buffer[*idx] = 0;
            if (!chat_command(buffer))
                write(TTYS0, buffer, *idx);
            *idx = 0;
        }
        return true;
    }
    return false;
}

bool
read_from_remote(char *buffer, int *idx)
{
    char ch;
    if (read(TTYS0, &ch, sizeof(char)))
    {
        buffer[(*idx)++] = ch;
		
        if (ch == '\n') 
        { 
           buffer[*idx] = 0; 
           printf("%s", buffer); 
           *idx = 0; 
        } 
        return true;
    }
    return false;
}

void
chat_init()
{
    exit = false;

    char in[256];
    char out[256];

    int in_idx = 0, out_idx = 0;
    bool vread;

    while (!exit)
    {
        vread = read_from_local(out, &out_idx);
        /* TODO: Could this lead to a race condition where it just reads from
         * local and not from remote? */
        if (!vread)
            vread = read_from_remote(in, &in_idx);
        if (!vread)
            _hlt();
    }
}

