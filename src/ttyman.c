#include "../include/ttyman.h"
#include "../include/tty.h"
#include "../include/kbd.h"
#include "../include/stdlib.h"
#include "../include/string.h"
#include "../include/kasm.h"

#include "../include/critical.h"

#define TTY_COUNT 9

static TTY * _ttys[TTY_COUNT];
static TTY * _active_tty = NULL;
static int _active_tty_idx = 0;

static int _x, _y, _width, _height;


void ttyman_keyboard_callback(char key_ascii, const char * keyboard_status)
{
	critical_enter();
		
	if (keyboard_status[SCANCODE_CHAR_LALT])
	{
		if (key_ascii >= '1' && key_ascii <= '9')
			ttyman_switch(key_ascii - '1');
		else if (keyboard_status[SCANCODE_CHAR_E])
			kbd_set_keymap("EN");
		else if (keyboard_status[SCANCODE_CHAR_S])
			kbd_set_keymap("ES");
		
		ttyman_update_info_text();
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
		_active_tty_idx = tty_idx;
		tty_display(_active_tty);
		ttyman_update_info_text();
	}
	
	critical_leave();
}


void ttyman_update_info_text(void)
{
	critical_enter();

	char info_string[128];
	char temp_string[128];
	
	strcpy(info_string, "TTY ");

	itoa(_active_tty_idx, temp_string, 10);	
	strcat(info_string, temp_string);

	strcat(info_string, " - Layout: ");
	strcat(info_string, kbd_get_keymap());

	vgatext_format_set(0x03);
	vgatext_strfill(_x, _y, _width, 1, info_string);
	

	critical_leave();
}

void ttyman_init(int x, int y, int width, int height)
{
	int i;
	char print_buffer[64];
	char * print_string;

	_x = x;
	_y = y;
	_width = width;
	_height = height;

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

