#ifndef _sh_h
#define _sh_c

void sh_init(void);
void sh_show_prompt(void);
int sh_read_command(void);
int sh_do_command(char buf[]);

int exec_laws(int argc, char **argv);
int exec_fortune(int argc, char **argv);

void buf_trim(char buf[]);

#endif
