

#include "../include/ttybox.h"
#include "../include/stdio.h"

char * ttybox_linearaddr(TTYBOX *ttybox, int linear)
{
	return ((linear >= 0 && linear < ttybox->length) ?
			ttybox->buffer + linear*2 : NULL);
}


void ttybox_newline(TTYBOX *ttybox)
{
	int i;
	char * posptr;

	for (i = 0; i < (ttybox->length - ttybox->width)*2; i++)
		ttybox->buffer[i] = ttybox->buffer[i+ttybox->width*2];

	for (i = 0; i < ttybox->width; i++)
		if ((posptr = ttybox_linearaddr(ttybox, ttybox->length - i - 1)))
		{
			posptr[0] = '\0';
			posptr[1] = DEFAULT_FORMAT;
		}

}

void ttybox_init(TTYBOX *ttybox, int x, int y, int width, int height, char * buffer)
{

	int i; 
	char * posptr;

	ttybox->x = x;
	ttybox->y = y;
	ttybox->width = width;
	ttybox->height = height;
	ttybox->length = width*height;
	ttybox->pos = 0;
	ttybox->buffer = buffer;
	ttybox->format = DEFAULT_FORMAT;

	for (i = 0; i < ttybox->length; i++)
		if ((posptr = ttybox_linearaddr(ttybox, i)))
		{
			posptr[0] = '\0';
			posptr[1] = DEFAULT_FORMAT;
		}
}

void ttybox_format_set(TTYBOX *ttybox, char format)
{
	ttybox->format = format;
}

void ttybox_putchar(TTYBOX *ttybox, char chr)
{
	char * posptr;

	switch (chr)
	{
	case '\n':
		if (ttybox->pos/ttybox->width == (ttybox->height-1))
			ttybox_newline(ttybox);
		else
			ttybox->pos += ttybox->width;

	case '\r':
		ttybox->pos = ttybox->pos - (ttybox->pos % ttybox->width);
		break;

	case '\b':
		if (ttybox->pos > 0)
		{
			ttybox->pos--;
			if ((posptr = ttybox_linearaddr(ttybox, ttybox->pos)))
				posptr[0] = '\0';
		}
		break;
	default:

		if ((posptr = ttybox_linearaddr(ttybox, ttybox->pos)))
		{
			posptr[0] = chr;
			posptr[1] = ttybox->format;
		}

		ttybox->pos++;

		if (ttybox->pos >= ttybox->length)
		{
			ttybox->pos--;
			ttybox_putchar(ttybox, '\n');
		}
	}
}

void ttybox_update_cursor(TTYBOX *ttybox)
{
	vgatext_cursor_setxy(ttybox->x + (ttybox->pos % ttybox->width),
						 ttybox->y + (ttybox->pos / ttybox->width));
}

void ttybox_display(TTYBOX *ttybox)
{
	vgatext_write(ttybox->buffer, ttybox->x, ttybox->y, ttybox->width, ttybox->height);
}
