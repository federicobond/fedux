

#include "../include/ttybox.h"
#include "../include/tty.h"
#include "../include/string.h"
#include "../include/mm.h"

#include "../include/kpanic.h"

#include "../include/kasm.h"

#define DEFAULT_MULTIPLIER 1

TTY *
tty_create(int x, int y, int width, int height)
{
	TTY * newtty = mm_malloc(sizeof(TTY));
	byte_queue * input_queue  = bq_create(width*height*DEFAULT_MULTIPLIER);
	byte_queue * input_display_queue = bq_create(width*height*DEFAULT_MULTIPLIER);
	TTYBOX *ttybox = ttybox_create(x, y, width, height);

	tty_init(newtty, input_queue, input_display_queue, ttybox, x, y, width, height);

	return newtty;
}

void
tty_destroy(TTY *tty)
{
	ttybox_destroy(tty->ttybox);
	bq_destroy(tty->input_display_queue);
	bq_destroy(tty->input_queue);
	mm_free(tty);
}

void
tty_init(TTY *tty,  
			  byte_queue *input_queue, 
			  byte_queue *input_display_queue,
			  TTYBOX *ttybox,
			  int x, int y, int width, int height)
{

	tty->input_queue = input_queue;
	tty->input_display_queue = input_display_queue;
	tty->ttybox = ttybox;
	tty->x = x;
	tty->y = y;
	tty->width = width;
	tty->height = height;
}

void
tty_input_write(TTY *tty, char *data, int size)
{
	int i, written;
	for (i = 0; i < size; i++)
	{
        /* TODO: Add support for ANSI escape codes for moving cursor, changing format, etc. */
		if (data[i] == '\b')
			bq_rread(tty->input_display_queue, NULL, 1);
		else
		{		
			written = bq_write_lossless(tty->input_display_queue, &data[i], sizeof(char));
			
			if (written && data[i] == '\n' && (bq_used(tty->input_display_queue) <= bq_avail(tty->input_queue)))
				bq_move(tty->input_display_queue, tty->input_queue, bq_used(tty->input_display_queue));				
		}
		ttybox_putchar(tty->ttybox, data[i], OWNER_SERVER);
	}
}

int
tty_input_read(TTY *tty, char *data, int size)
{
	return bq_read(tty->input_queue, data, size);
}

int
tty_output_write(TTY *tty, char *data, int size)
{
	ttybox_write(tty->ttybox, data, size, OWNER_CLIENT);
	return size;
}

void
tty_display(TTY *tty)
{
	ttybox_display(tty->ttybox);
	ttybox_update_cursor(tty->ttybox);
}

