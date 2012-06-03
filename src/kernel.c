#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/kc.h"
#include "../include/sh.h"
#include "../include/vgatext.h"

DESCR_INT idt[0xA];			/* IDT de 10 entradas*/
IDTR idtr;				/* IDTR */

int tickpos = 0;
int cursor = 0;

void timertick_handler() 
{
    char *video = (char *) 0xb8000;
    video[tickpos]= '*';
    tickpos += 2;
}


/**********************************************
kmain() 
Punto de entrada de cóo C.
*************************************************/
int
kmain(void)
{

	_cli();

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
	vgatext_charfill(0, 0, 79, 6, 0);
	vgatext_format_set(0x3F);
	vgatext_strfill(1, 1, 77, 4, "Welcome to Fedux! Press spacebar to get the prompt... (It's not buggy, it's just the way it's done)");

    /*sh_init();
	*/
    while(1)
		_hlt();
	
}

