#ifndef _KBD_H
#define _KBD_H

#include "scancode.h"

#define NPRTBL 0
#define KEYCOUNT 128

#define LSHIFT (1<<1)
#define RSHIFT (1<<2)
#define CAPSLOCK (1<<3)

typedef void (*kbd_callback_t)(char, const char *);

void kbd_init(kbd_callback_t event_callback);

int kbd_set_keymap(const char *code);
const char * kbd_get_keymap();

#endif
