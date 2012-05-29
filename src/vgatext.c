

int _width, _height;
int _current_format;

int _cursor_x, _cursor_y;

char * _memory_start;
char * _memory_end;


char * vgatext_posaddr(int x, int y)
{
	return _memory_start + y*width + x;
}

int vgatext_init(int width, int height, char * memory_start)
{
	_width = width;
	_height = height;
	_memory_start = memory_start;
	_memory_end = memory_start + width*height*2;
}


int vgatext_print(int x, int y, char * text)
{
	char * print_addr = vgatext_posaddr(x, y);

	while (print_addr <= _memory_end && *text)
	{
		print_addr[0] = *text;
		print_addr[1] = _current_format;
		print_addr += 2;
	}
}

int vgatext_textbox(int x, int y, int width, int height, char * text)
{
	
}

int vgatext_set_format(int front_color, int back_color)
{
}


void vgatext_cursor_enable(int enable)
{
	
}


void vgatext_cursor_set(int x, int y)
{
	_cursor_x = x;
	_cursor_y = y;
}


void vgatext_cursor_get(int *x, int *y)
{
	*x = _cursor_x;
	*y = _cursor_y;	
}

