

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
	bq_write(tty->input_queue, data, size);
}

void tty_input_read(TTY *tty, char *data, int size)
{
	bq_read(tty->input_queue, data, size);
	
	/* Echo */
	bq_write(tty->output_queue, data, size);
}



void tty_display(TTY *tty)
{
	/* Shallow copy of the output queue to work with rotating indexes */
	byte_queue tmp_output_queue = *(tty->output_queue);

	/* Shallow copy of the input queue to work with rotating indexes */
	byte_queue tmp_input_queue = *(tty->input_queue);

	/* Temporary video buffer to store display data */
	char video_buffer[MAXBUFFSIZE];
	byte_queue video_queue;

	char line_buffer[MAXBUFFSIZE];
	

	char outchar;
	char format = 0x0F;

	bq_init(&video_queue, video_buffer, tty->height*tty->width*2);

	for (outchar = 0; outchar < 32; outchar++)
		video_buffer[outchar] = 0;

	
	while (bq_read(&tmp_output_queue, &outchar, 1))
	{
		switch (outchar)
		{
		case '\n':
			outchar = '\0';
			bq_write(&video_queue, &outchar, 1);
			while (bq_used(&video_queue) % (tty->width*2))
				bq_write(&video_queue, &outchar, 1);
			break;
		case '\t':
			outchar = '\0';
			bq_write(&video_queue, &outchar, 1);
			while (bq_used(&video_queue) % (TABSIZE*2))
				bq_write(&video_queue, &outchar, 1);
			break;
		default:
			bq_write(&video_queue, &outchar, 1);
			bq_write(&video_queue, &format, 1);
		}
	}

	

	/* Echo */
	while (bq_read(&tmp_input_queue, &outchar, 1))
	{
		switch (outchar)
		{
		case '\n':
			outchar = '\0';
			bq_write(&video_queue, &outchar, 1);
			while (bq_get_write(&video_queue) % (tty->width*2))
				bq_write(&video_queue, &outchar, 1);
			break;
		case '\t':
			outchar = '\0';
			bq_write(&video_queue, &outchar, 1);
			while (bq_used(&video_queue) % (TABSIZE*2))
				bq_write(&video_queue, &outchar, 1);
			break;
		default:
			bq_write(&video_queue, &outchar, 1);
			bq_write(&video_queue, &format, 1);
		}
	}

	outchar = '\0';
	while (bq_get_write(&video_queue) % (tty->width*2))
				bq_write(&video_queue, &outchar, 1);


	vgatext_writebq(&video_queue, tty->x, tty->y, tty->width);

	
	
}
