#ifndef _TTYMAN_H
#define _TTYMAN_H

void ttyman_keyboard_callback(char key_ascii, const char * keyboard_status);
void ttyman_switch(int tty_idx);
void ttyman_init(int x, int y, int width, int height);
int ttyman_write(char * data, int size);
int ttyman_read(char * data, int size);

#endif
