#include "../include/kasm.h"


#define TTY_DEBUG

#ifdef TTY_DEBUG

#include "../include/bq.h"
#include "../include/tty.h"


char _input_buffer[MAXBUFFSIZE];
char _output_buffer[MAXBUFFSIZE];

byte_queue _input_queue;
byte_queue _output_queue;

TTY _tty;

int tty_initialized = 0;


#endif


unsigned char scancodes[128] =
{
	0,  27,
	'1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
	'-', '=', 
	'\b',	/* Backspace */
	'\t',	/* Tab */
 	'q', 'w', 'e', 'r',	't', 'y', 'u', 'i', 'o', 'p',
	'[', ']', '\n',	/* Enter key */
	0,			/* 29   - Control */
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
	'\'', '`',   0,		/* Left shift */
	'\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',   0,				/* Right shift */
	'*',
	0,	/* Alt */
	' ',	/* Space bar */
	0,	/* Caps lock */
	0,	/* 59 - F1 key ... > */
	0,   0,   0,   0,   0,   0,   0,   0,
	0,	/* < ... F10 */
	0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

static int tickpos = 0;


void keyboard_handler()
{
	int scancode = _inb(0x60);
	char decoded = 0;

	/* Only when pressed */

	if (!(scancode & 0x80))
    	decoded = scancodes[scancode];

#ifdef TTY_DEBUG
	if (decoded)
	{
		if (!tty_initialized)
		{
			bq_init(&_input_queue, _input_buffer, MAXBUFFSIZE);
			bq_init(&_output_queue, _output_buffer, MAXBUFFSIZE);
			tty_init(&_tty, &_input_queue, &_output_queue, 0, 0, 80, 3);
			tty_initialized += 1;
			vgatext_print(80*25-160, "Initialized");
		}

		tty_input_write(&_tty, &decoded, 1);
		tty_display(&_tty);

		vgatext_print(80*25-80, _input_buffer);
	}


#endif
}

