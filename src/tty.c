

#include "../include/ttybox.h"
#include "../include/tty.h"
#include "../include/string.h"

void tty_init(TTY *tty,  byte_queue *input_queue, byte_queue *output_queue,
			  int x, int y, int width, int height)
{

	tty->input_queue = input_queue;
	tty->output_queue = output_queue;
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
			bq_write(tty->input_queue, data, size);
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

	/* Temporary video buffer to store display data */
	char video_buffer[MAXBUFFSIZE];

	TTYBOX ttybox;

	ttybox_init(&ttybox, tty->x, tty->y, tty->width, tty->height, video_buffer);
	ttybox_format_set(&ttybox, 0x0F);

	while (bq_read(&tmp_output_queue, &outchar, 1))
		ttybox_putchar(&ttybox, outchar);
	
	while (bq_read(&tmp_input_queue, &outchar, 1))
		ttybox_putchar(&ttybox, outchar);

	ttybox_display(&ttybox);
	ttybox_update_cursor(&ttybox);
	
}
