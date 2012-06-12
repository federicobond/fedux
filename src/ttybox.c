#include "../include/ttybox.h"
#include "../include/stdio.h"
#include "../include/mm.h"

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

	ttybox->client_pos -= ttybox->width;

	if (ttybox->client_pos < 0)
		ttybox->client_pos = -1;

}

TTYBOX * ttybox_create(int x, int y, int width, int height)
{
	char *buffer = mm_malloc(width*height*2);
	TTYBOX *ttybox = mm_malloc(sizeof(TTYBOX));
	ttybox_init(ttybox, x, y, width, height, buffer);
	return ttybox;
}

void ttybox_destroy(TTYBOX *ttybox)
{
	mm_free(ttybox->buffer);
	mm_free(ttybox);
}

void ttybox_init(TTYBOX *ttybox, int x, int y, int width, int height, char * buffer)
{
	ttybox->x = x;
	ttybox->y = y;
	ttybox->width = width;
	ttybox->height = height;
	ttybox->length = width*height;
	ttybox->buffer = buffer;
	

	ttybox_clear(ttybox);
}


void ttybox_clear(TTYBOX *ttybox)
{
	int i; 
	char * posptr;
	
	ttybox->pos = 0;
	ttybox->format = DEFAULT_FORMAT;

	for (i = 0; i < ttybox->length; i++)
		if ((posptr = ttybox_linearaddr(ttybox, i)))
		{
			posptr[0] = '\0';
			posptr[1] = DEFAULT_FORMAT;
		}
}

void ttybox_pos_set(TTYBOX *ttybox, int pos, char owner)
{
	if (pos >= ttybox->length)
	{
		ttybox_newline(ttybox);
		pos = ttybox->length - ttybox->width;
	}

	ttybox->pos = pos;
	if (owner == OWNER_CLIENT)
		ttybox->client_pos = pos;
}

int ttybox_pos_get(TTYBOX *ttybox)
{
	return ttybox->pos;
}

void ttybox_format_set(TTYBOX *ttybox, char format)
{
	ttybox->format = format;
}

void ttybox_puts(TTYBOX *ttybox, char * str, char owner)
{
	while (*str)
		ttybox_putchar(ttybox, *(str++), owner);
}

void ttybox_write(TTYBOX *ttybox, char * data, int size, char owner)
{
	while (size--)
		ttybox_putchar(ttybox, *(data++), owner);
}


int ttybox_erasable(TTYBOX *ttybox, int pos, char owner)
{
	return (pos > 0 &&
			pos < ttybox->length &&
			(owner == OWNER_CLIENT ||
			(owner == OWNER_SERVER && pos > ttybox->client_pos)));
}

void ttybox_putchar(TTYBOX *ttybox, char chr, char owner)
{
	char * posptr;

	switch (chr)
	{
	case '\n':
		ttybox_pos_set(ttybox, ttybox_pos_get(ttybox) + ttybox->width, owner);
        /* Fallthrough */
	case '\r':
		ttybox_pos_set(ttybox, (ttybox_pos_get(ttybox)/ttybox->width)*ttybox->width, owner);
		break;
	case '\b':
		if (ttybox_erasable(ttybox, ttybox_pos_get(ttybox), owner))
		{
			ttybox_pos_set(ttybox, ttybox_pos_get(ttybox)-1, owner);
			if ((posptr = ttybox_linearaddr(ttybox, ttybox_pos_get(ttybox))))
				posptr[0] = '\0';
		}
		break;
    case '\t':
        /* Do nothing on tab, reserved for autocomplete */
        break;
	default:

		if ((posptr = ttybox_linearaddr(ttybox, ttybox_pos_get(ttybox))))
		{
			posptr[0] = chr;
			posptr[1] = ttybox->format;
		}

		ttybox_pos_set(ttybox, ttybox_pos_get(ttybox) + 1, owner);
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
