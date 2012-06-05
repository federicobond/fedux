#ifndef _SH_H
#define _SH_H

void sh_init(void);
void sh_show_prompt(void);
void sh_read_command(char buf[]);
int sh_do_command(char buf[]);
void sh_tokenize(char buf[], int *argc, char *argv[]);

int exec_laws(int argc, char **argv);
int exec_fortune(int argc, char **argv);
int exec_echo(int argc, char **argv);

void buf_trim(char buf[]);

#endif
