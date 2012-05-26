#include "../include/kasm.h"
#include "../include/defs.h"

DESCR_INT idt[0xA];			/* IDT de 10 entradas*/
IDTR idtr;				/* IDTR */

int tickpos=0;

void timertick() {


    char *video = (char *) 0xb8000;
    video[tickpos]= '*';
    tickpos += 2;

    unsigned int tmp = tickpos / 2;
    _outb((char)14, (char *)0x3D4);
    _outb((char)(tmp >> 8), (char *)0x3D5);
    _outb((char)15, (char *)0x3D4);
    _outb((char)tmp, (char *)0x3D5);

}

/**********************************************
kmain() 
Punto de entrada de cóo C.
*************************************************/

kmain() 
{

        int i,num;

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
        
	_sti();

        while(1)
        {
        }
	
}

