#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/kc.h"

#include "vgatext.h"

DESCR_INT idt[0xA];			/* IDT de 10 entradas*/
IDTR idtr;				/* IDTR */

int tickpos = 0;

int cursor = 0;

void print(char *msg)
{

	char *video = (char *) 0xb8000;
	while (*msg != 0)
	{
		video[cursor] = *msg++;
		cursor += 2;
	}
}

void video_init()
{


	int reg_vrer;
	int reg_mor;

	return;

	reg_mor = _inb(0x03CC);
	reg_mor = reg_mor | 0x01;
	_outb(reg_mor, 0x03C2);

	_outb(0x11, 0x3D4);	
	reg_vrer = _inb(0x03D5);

	if (reg_vrer & 0x80)
		print("protected");
	else
		print("not protected");


	reg_vrer = reg_vrer & 0x7F;

	_outb(reg_vrer, 0x03D5);



}

void timertick() 
{

    char *video = (char *) 0xb8000;

    unsigned int tmp = tickpos / 2;

/*    _set_cursor(tmp); */


    _outb(0x0E, 0x03D4);
    _outb(tmp >> 8, 0x03D5);
    _outb(0x0F, 0x03D4);
    _outb(tmp, 0x03D5);

    video[tickpos]= '*';
    tickpos += 2;


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
	
/* Carga de IDTR    */

	idtr.base = 0;  
	idtr.base +=(dword) &idt;
	idtr.limit = sizeof(idt)-1;
	
	_lidt (&idtr);	

	_cli();

/* Habilito interrupcion de timer tick*/

        _mask_pic_1(0xFE);
        _mask_pic_2(0xFF);
        
/* Inicializo la placa de video */

	/*video_init();
	*/

	_sti();


	vgatext_init(80, 25, (char *) 0xB8000);
	vgatext_cursor_set(0, 0);
	vgatext_clear();
	
	/*
	vgatext_format_set(0xFF);
	vgatext_charfill(19, 4, 61, 21, 0);
	vgatext_format_set(0xF0);
	vgatext_strfill(20, 5, 60, 20, "Welcome to Fedux!");
	*/
        while(1)
        {
        }
	
}

