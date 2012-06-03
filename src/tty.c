

#include "../include/ttybox.h"
#include "../include/tty.h"
#include "../include/string.h"
#include "../include/mm.h"

#define DEFAULT_MULTIPLIER 2

TTY * tty_create(int x, int y, int width, int height)
{
	TTY * newtty = mm_malloc(sizeof(TTY));
	byte_queue * input_queue  = bq_create(width*height*DEFAULT_MULTIPLIER);
	byte_queue * output_queue = bq_create(width*height*DEFAULT_MULTIPLIER);
	TTYBOX *ttybox = ttybox_create(x, y, width, height);

	tty_init(newtty, input_queue, output_queue, ttybox, x, y, width, height);

	return newtty;
}

void tty_destroy(TTY *tty)
{
	ttybox_destroy(tty->ttybox);
	bq_destroy(tty->output_queue);
	bq_destroy(tty->input_queue);
	mm_free(tty);
}

void tty_init(TTY *tty,  
			  byte_queue *input_queue, 
			  byte_queue *output_queue,
			  TTYBOX *ttybox,
			  int x, int y, int width, int height)
{

	tty->input_queue = input_queue;
	tty->output_queue = output_queue;
	tty->ttybox = ttybox;
	tty->x = x;
	tty->y = y;
	tty->width = width;
	tty->height = height;
}

void tty_input_write(TTY *tty, char *data, int size)
{
	int i;
	for (i = 0; i < size; i++)
		if (data[i] == '\b')
			bq_rread(tty->input_queue, NULL, 1);
		else	
			bq_write_lossless(tty->input_queue, &data[i], sizeof(char));
}

void tty_input_read(TTY *tty, char *data, int size)
{
	bq_read(tty->input_queue, data, size);
	
	/* Echo */
	bq_write(tty->output_queue, data, size);
}

void tty_output_write(TTY *tty, char *data, int size)
{
	bq_write(tty->output_queue, data, size);
}

void tty_display(TTY *tty)
{
	char outchar;

	/* Shallow copy of the output queue to work with rotating indexes */
	byte_queue tmp_output_queue = *(tty->output_queue);

	/* Shallow copy of the input queue to work with rotating indexes */
	byte_queue tmp_input_queue = *(tty->input_queue);

	ttybox_clear(tty->ttybox);
	ttybox_format_set(tty->ttybox, 0x0F);

	while (bq_read(&tmp_output_queue, &outchar, sizeof(char)))
		ttybox_putchar(tty->ttybox, outchar);
	
	while (bq_read(&tmp_input_queue, &outchar, sizeof(char)))
		ttybox_putchar(tty->ttybox, outchar);

	ttybox_display(tty->ttybox);
	ttybox_update_cursor(tty->ttybox);
	
}
