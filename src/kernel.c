#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/kc.h"

#include "vgatext.h"

DESCR_INT idt[0xA];			/* IDT de 10 entradas*/
IDTR idtr;				/* IDTR */

int tickpos = 0;

int cursor = 0;


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

void print(char *msg)
{

	char *video = (char *) 0xb8000;
	while (*msg != 0)
	{
		video[cursor] = *msg++;
		cursor += 2;
	}
}


void timertick() 
{


	

    char *video = (char *) 0xb8000;
    video[tickpos]= '*';
    tickpos += 2;


}

void keyboard()
{

	char *video = (char *) 0xb8000;
	int scancode = _inb(0x60);

	/* Only when pressed */

	if (!(scancode & 0x80))
	{
		scancode &= 0x7F;
    	video[tickpos]= scancodes[scancode];
    	tickpos += 2;
	}

}


/**********************************************
kmain() 
Punto de entrada de cóo C.
*************************************************/

void kmain() 
{

/* Borra la pantalla. */ 

	k_clear_screen();


/* CARGA DE IDT CON LA RUTINA DE ATENCION DE IRQ0    */

        setup_idt_entry(&idt[0x08], 0x08, (dword)&_timertick_handler, ACS_INT, 0);
		setup_idt_entry(&idt[0x09], 0x08, (dword)&_keyboard_handler, ACS_INT, 0);
	
/* Carga de IDTR    */

	idtr.base = 0;  
	idtr.base +=(dword) &idt;
	idtr.limit = sizeof(idt)-1;
	
	_lidt (&idtr);	

	_cli();

/* Habilito interrupcion de timer tick*/

        _mask_pic_1(0xFD);
        _mask_pic_2(0xFF);
        
/* Inicializo la placa de video */

	/*video_init();
	*/

	_sti();


	vgatext_init(80, 25, (char *) 0xB8000);
	vgatext_cursor_setxy(0, 0);
	vgatext_clear();
	
	
	vgatext_format_set(0x70);
	vgatext_charfill(20, 4, 31, 11, 0);
	vgatext_format_set(0x3F);
	vgatext_strfill(21, 5, 29, 9, "Welcome to Fedux! This is not free software (yet), and you are not supposed to copy and distribute it!");
	
        while(1)
        {
        }
	
}

