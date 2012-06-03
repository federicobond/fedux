#ifndef _KBD_H
#define _KBD_H

void kbd_init(void);
int  kbd_set_keymap(char *code);
int  kbd_read(char *buf, int size);
bool kbd_shifted(void);
bool kdb_capslocked(void);
char kbd_keymap_get(unsigned int scancode);
void keyboard_handler(void);

#endif
