#ifndef _TTYBOX_H
#define _TTYBOX_H

#include "../include/vgatext.h"

#define OWNER_SERVER 0
#define OWNER_CLIENT 1

typedef struct TTYBOX
{
	int x, y, width, height;
	int length, pos, client_pos;
	char format;
	char * buffer;

} TTYBOX;

void ttybox_init(TTYBOX *ttybox, int x, int y, int width, int height, char * buffer);
void ttybox_clear(TTYBOX *ttybox);
void ttybox_format_set(TTYBOX *ttybox, char format);
void ttybox_putchar(TTYBOX *ttybox, char chr, char owner);
void ttybox_puts(TTYBOX *ttybox, char * str, char owner);
void ttybox_write(TTYBOX *ttybox, char * data, int size, char owner);

void ttybox_display(TTYBOX *ttybox);
void ttybox_update_cursor(TTYBOX *ttybox);

TTYBOX * ttybox_create(int x, int y, int width, int height);
void ttybox_destroy(TTYBOX *ttybox);


#endif
