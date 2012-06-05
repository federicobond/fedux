#ifndef _TTY_H
#define _TTY_H

#include "../include/vgatext.h"
#include "../include/bq.h"

#define MAXBUFFSIZE 80*25*2*2
#define TTYCOUNT 4

#define TABSIZE 5

#include "ttybox.h"

typedef struct TTY
{
	byte_queue *input_queue;
	byte_queue *input_display_queue;
	byte_queue *output_queue;
	TTYBOX *ttybox;
	unsigned int x, y, width, height;
} TTY;


void tty_init(TTY *tty,  
			  byte_queue *input_queue, 
			  byte_queue *input_display_queue,
			  byte_queue *output_queue,
			  TTYBOX *ttybox,
			  int x, int y, int width, int height);

void tty_input_write(TTY *tty, char *data, int size);

int tty_input_read(TTY *tty, char *data, int size);

int tty_output_write(TTY *tty, char *data, int size);

void tty_display(TTY *tty);


#endif
