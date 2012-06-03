#include "../include/stdlib.h"
#include "../include/stddef.h"
#include "../include/vgatext.h"
#include "../include/kasm.h"
#include "../include/bq.h"



int _width, _height;
char _current_format;

int _cursor_x, _cursor_y;

char * _memory_start;
char * _memory_end;


int vgatext_poslinear(int x, int y)
{
	return (x >= 0 && y >= 0 && x < _width && y < _height ?
			  y*_width + x : INVALID_POSITION);
}


void vgatext_linearpos(int linear, int *x, int *y)
{
	if (linear >= 0 && linear < _width*_height)
	{
		*x = linear%_width;
		*y = linear/_width;
	}
	else
	{
		*x = INVALID_POSITION;
		*y = INVALID_POSITION;
	}
			
}

char * vgatext_linearaddr(int linear)
{
	return ((linear >= 0 && linear < _width*_height) ?
			_memory_start + linear*2 : NULL);
}


int vgatext_addrlinear(char * addr)
{
	return ((addr >= _memory_start && addr <= _memory_end) ?
			(addr - _memory_start)/2 : INVALID_POSITION);
}


char * vgatext_posaddr(int x, int y)
{
	return (x >= 0 && y >= 0 && x < _width && y < _height ?
			  _memory_start + (y*_width + x)*2 : NULL);
}


void vgatext_addrpos(char * addr, int *x, int *y)
{
	if (addr >= _memory_start && addr <= _memory_end)
	{
		*x = ((addr - _memory_start)/2)%_width;
		*y = ((addr - _memory_start)/2)/_width;
	}
	else
	{
		*x = INVALID_POSITION;
		*y = INVALID_POSITION;
	}
}

bool vgatext_init(int width, int height, char * memory_start)
{
	_width = width;
	_height = height;
	_memory_start = memory_start;
	_memory_end = memory_start + width*height*2 - 1;
	return true;
}


int vgatext_print(int linear, char * text)
{

	while (*text && vgatext_putcharl(linear, *text))
	{
		text++;
		linear++;
	}
	
	return linear; 		/* Return final linear address */
}


void vgatext_clear()
{
	int format = vgatext_format_get();
	vgatext_format_set(DEFAULT_FORMAT);
	vgatext_charfill(0, 0, _width, _height, 0);
	vgatext_format_set(format);
}

int vgatext_putcharxy(int x, int y, char chr)
{
	char * posaddr = vgatext_posaddr(x, y);
	
	if (posaddr)
	{
		posaddr[0] = chr;
		posaddr[1] = _current_format;
	}

	return (posaddr != NULL);
}

int vgatext_putcharl(int linear, char chr)
{
	char * posaddr = vgatext_linearaddr(linear);
	
	if (posaddr)
	{
		posaddr[0] = chr;
		posaddr[1] = _current_format;
	}

	return (posaddr != NULL);
}


void vgatext_charfill(int start_x, int start_y, int width, int height, char fillchar)
{
	int x, y;
	int end_x, end_y;
	end_x = start_x + width;
	end_y = start_y + height;

	for (y = start_y; y < end_y; y++)
		for (x = start_x; x < end_x; x++)
			vgatext_putcharxy(x, y, fillchar);
}


void vgatext_strfill(int start_x, int start_y, int width, int height, char * str)
{

	int x, y;
	int end_x, end_y;

	end_x = start_x + width;
	end_y = start_y + height;


	for (y = start_y; y < end_y; y++)
		for (x = start_x; x < end_x; x++)
		{

			vgatext_putcharxy(x, y, *str);

			/*
			 Continue exploring text only if it has not finished
			*/
			if (*str)
				str++;
		}	
	
}

void vgatext_writebq(byte_queue *bq, int dest_x, int dest_y, int width)
{
	byte_queue tmp_bq = *bq;

	int i = 0;	
	char * posaddr;
	
	while (bq_used(&tmp_bq))
	{
		posaddr = vgatext_posaddr((i % width) + dest_x, (i / width) + dest_y);
		
		/*	Read 2 bytes from queue.
			Note: If posaddr is NULL then bq makes a fake read,
				  so it is as if the character appears off screen.
		*/
		bq_read(&tmp_bq, posaddr, 2);

		i++;	
	}
	
}

void vgatext_write(char * buffer, int dest_x, int dest_y, int width, int height)
{
	int x, y;
	char * posaddr;

	for (y = 0; y < height; y++)
		for (x = 0; x < width; x++)
		{
			if ((posaddr = vgatext_posaddr(x + dest_x, y + dest_y)))
			{
				posaddr[0] = buffer[0];
				posaddr[1] = buffer[1];
			}

			buffer += 2;		
		}
}

void vgatext_format_set(int format)
{
	_current_format = format;
}

int vgatext_format_get()
{
	return _current_format;
}


void vgatext_cursor_enable()
{
}

void vgatext_cursor_disable()
{
}


void vgatext_cursor_setxy(int x, int y)
{

	int linear_pos = vgatext_poslinear(x, y);


	if (linear_pos != INVALID_POSITION)
	{
		_cursor_x = x;
		_cursor_y = y;
	
		_outb(0x0E, 0x03D4);
	    _outb(linear_pos >> 8, 0x03D5);
	    _outb(0x0F, 0x03D4);
	    _outb(linear_pos, 0x03D5);
	}

}

void vgatext_cursor_setl(int linear)
{
	int x, y;
	vgatext_linearpos(linear, &x, &y);
	vgatext_cursor_setxy(x, y);
}

void vgatext_cursor_getxy(int *x, int *y)
{
	*x = _cursor_x;
	*y = _cursor_y;
}

void vgatext_cursor_getl(int *linear)
{
	*linear = vgatext_poslinear(_cursor_x, _cursor_y);
}

