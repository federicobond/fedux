
#include "./stdlib.h"
#include "vgatext.h"
#include "../include/kasm.h"


#define INVALID_POSITION (-1)

#define DEFAULT_FORMAT 0x0F


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

int vgatext_init(int width, int height, char * memory_start)
{
	_width = width;
	_height = height;
	_memory_start = memory_start;
	_memory_end = memory_start + width*height*2 - 1;
	return TRUE;
}


int vgatext_rawprint(int linear, char * text)
{
	char * print_addr = vgatext_linearaddr(linear);

	if (print_addr)
	{
		while (print_addr <= (_memory_end-1) && *text)
		{
			print_addr[0] = *text;
			print_addr[1] = _current_format;
			print_addr += 2;
		}
	}

	return vgatext_addrlinear(print_addr); 		/* Return final linear address */
}


void vgatext_clear()
{
	int format = vgatext_format_get();
	vgatext_format_set(DEFAULT_FORMAT);
	vgatext_charfill(0, 0, _width, _height, 0);
	vgatext_format_set(format);
}


void vgatext_charfill(int start_x, int start_y, int width, int height, char fillchar)
{
	int x, y;
	int end_x, end_y;
	char * posaddr;

	end_x = start_x + width;
	end_y = start_y + height;


	for (y = start_y; y <= end_y; y++)
		for (x = start_x; x <= end_x; x++)
		{
			if ((posaddr = vgatext_posaddr(x, y)))
			{
				posaddr[0] = fillchar;
				posaddr[1] = _current_format;
			}
		}
}


void vgatext_strfill(int start_x, int start_y, int width, int height, char * str)
{

	int x, y;
	int end_x, end_y;
	char * posaddr;

	end_x = start_x + width;
	end_y = start_y + height;


	for (y = start_y; y <= end_y; y++)
		for (x = start_x; x <= end_x; x++)		
		{

			/* Output text and format only
			   if position is valid.
			*/
			if ((posaddr = vgatext_posaddr(x, y)))
			{
				posaddr[0] = *str;
				posaddr[1] = _current_format;
			}

			/*
			 Continue exploring text only if it has not finished
			*/
			if (*str)
				str++;
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

