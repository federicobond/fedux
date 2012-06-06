#include "../include/tty.h"
#include "../include/kbd.h"
#include "../include/stdlib.h"
#include "../include/string.h"
#include "../include/kasm.h"

#include "../include/critical.h"

#define TTY_COUNT 4

static TTY * _ttys[TTY_COUNT];
static TTY * _active_tty = NULL;


void ttyman_keyboard_callback(char key_ascii, char * keyboard_status)
{
	critical_enter();
		
	if (keyboard_status[SCANCODE_CHAR_LALT])
	{
		ttyman_switch(key_ascii - '1');
	}
	else if (_active_tty && key_ascii)
	{
		tty_input_write(_active_tty, &key_ascii, sizeof(char));
		tty_display(_active_tty);
	}
	
	critical_leave();
}


void ttyman_switch(int tty_idx)
{

	critical_enter();

	if (tty_idx >= 0 && tty_idx < TTY_COUNT)
	{
		_active_tty = _ttys[tty_idx];
		tty_display(_active_tty);
	}
	
	critical_leave();
}

void ttyman_init(int x, int y, int width, int height)
{
	int i;
	char print_buffer[64];
	char * print_string;

	critical_enter();

	for (i = 0; i < TTY_COUNT; i++)
	{
		_ttys[i] = tty_create(x, y + 1, width, height - 1);
		print_string = "Welcome. This is TTY ";
		tty_output_write(_ttys[i], print_string, strlen(print_string));
		itoa(i+1, print_buffer, 10);
		print_string = print_buffer;
		tty_output_write(_ttys[i], print_string, strlen(print_string));
		print_string = ".\n";
		tty_output_write(_ttys[i], print_string, strlen(print_string));
	}

	ttyman_switch(0);

	kbd_init(ttyman_keyboard_callback);

	critical_leave();
	
}


int ttyman_write(char * data, int size)
{
	int written;

	critical_enter();

	written = tty_output_write(_active_tty, data, size);
	tty_display(_active_tty);
	
	critical_leave();

	return written;
}

int ttyman_read(char * data, int size)
{

	int retval;

	critical_enter();

	retval = tty_input_read(_active_tty, data, size);

	critical_leave();

	return retval;

}

