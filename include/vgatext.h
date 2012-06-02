#ifndef _VGATEXT_H
#define _VGATEXT_H

#include "stddef.h"

bool vgatext_init(int width, int height, char * memory_start);

int vgatext_rawprint(int linear, char * text);

void vgatext_clear();

void vgatext_charfill(int start_x, int start_y, int width, int height, char fillchar);

void vgatext_strfill(int start_x, int start_y, int width, int height, char * str);

void vgatext_format_set(int format);
int	 vgatext_format_get();

void vgatext_cursor_setxy(int x, int y);
void vgatext_cursor_setl(int linear);

void vgatext_cursor_getxy(int *x, int *y);
void vgatext_cursor_getl(int *linear);

void vgatext_cursor_disable();
void vgatext_cursor_enable();


char * vgatext_posaddr(int x, int y);
void vgatext_addrpos(char * addr, int *x, int *y);

#endif
