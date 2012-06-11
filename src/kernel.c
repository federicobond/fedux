#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/sh.h"
#include "../include/vgatext.h"
#include "../include/kbd.h"
#include "../include/multiboot.h"
#include "../include/kpanic.h"
#include "../include/stdlib.h"
#include "../include/mm.h"
#include "../include/tty.h"
#include "../include/ttyman.h"
#include "../include/ttybox.h"
#include "../include/serialman.h"
#include "../include/syscall.h"
#include "../include/critical.h"
#include "../include/stdio.h"

#define KERNEL_RELEASE "1.0 (Clumsy Padawan)"


DESCR_INT idt[0x100];	/* IDT de 256 entradas*/
IDTR idtr;				/* IDTR */


TTYBOX * firstbox = NULL;

/***************************************************************
*setup_idt_entry
* Inicializa un descriptor de la IDT
*
*Recibe: Puntero a elemento de la IDT
*	 Selector a cargar en el descriptor de interrupcion
*	 Puntero a rutina de atencion de interrupcion	
*	 Derechos de acceso del segmento
*	 Cero
*
*	 Repudio a "libc.c"
****************************************************************/

void setup_idt_entry (DESCR_INT *item, byte selector, dword offset, byte access,
			 byte cero) {
  item->selector = selector;
  item->offset_l = offset & 0xFFFF;
  item->offset_h = offset >> 16;
  item->access = access;
  item->cero = cero;
}

void timertick_handler() 
{
}

void keyboard_callback(char ascii, char * keyboard_status)
{
	static int linear = 0;
	char str[16];
	itoa(ascii, str, 16);

	if (keyboard_status[SCANCODE_CHAR_F1] &&
		keyboard_status[SCANCODE_CHAR_LALT])
			linear = vgatext_print(linear,"ALT+F1");

}



/**********************************************
kmain() 
Punto de entrada de cóo C.
*************************************************/
int kmain(multiboot_info_t *mbi, unsigned long int magic)
{

	critical_enter();

/* CARGA DE IDT CON LA RUTINA DE ATENCION DE IRQ0    */

    setup_idt_entry(&idt[0x08], 0x08, (dword)&_timertick_handler, ACS_INT, 0);
    setup_idt_entry(&idt[0x09], 0x08, (dword)&_keyboard_handler, ACS_INT, 0);

	setup_idt_entry(&idt[0x0B], 0x08, (dword)&_serial_handler, ACS_INT, 0);
    setup_idt_entry(&idt[0x0C], 0x08, (dword)&_serial_handler, ACS_INT, 0);
	

	setup_idt_entry(&idt[0x80], 0x08, (dword)&_syscall_handler, ACS_INT, 0);
	
/* Carga de IDTR    */

	idtr.base = 0;  
	idtr.base +=(dword) &idt;
	idtr.limit = sizeof(idt)-1;
	
	_lidt (&idtr);	

/* Habilito interrupcion de timer tick*/

    _mask_pic_1(~((1 << 0) | (1 << 1) | (1 << 3) | (1 << 4)));
    _mask_pic_2(0xFF);

/* Initialize memory management */

	mm_setup(mbi);


/* Initialize keyboard driver */
/*	kbd_init(keyboard_callback);*/

        
/* Initialize video driver */

	/*video_init();
	*/

	vgatext_init(80, 25, (char *) 0xB8000);
	vgatext_cursor_setxy(0, 0);
	vgatext_clear();


/* Initialize TTY manager */

	ttyman_init(0, 1, 80, 24);

/* Initialize serial port manager */

	serialman_init();

/* Initialize basic ttybox output */

	firstbox = ttybox_create(0, 9, 80, 17);
	
/* Initialize first row of text */
	vgatext_format_set(0x70);
	vgatext_charfill(0, 0, 80, 1, '\0');
	vgatext_print(vgatext_poslinear(0, 0), "Fedux");
	vgatext_print(vgatext_poslinear(6, 0), KERNEL_RELEASE);
	vgatext_format_set(0x0F);
	

/* End of critical initializations: Re-enable interrupts */
	critical_leave();

	_hlt();

    sh_init();

	return 0;
	
}


void print_memory_map(int linear, multiboot_info_t *mbi)
{
	int i = 0;
	char print_buffer[64];	
	multiboot_memory_map_t *mmap = (multiboot_memory_map_t *)mbi->mmap_addr;

	ttybox_puts(firstbox, "kmain starts at ", OWNER_CLIENT);

	itoa((int)kmain, print_buffer, 16);
	ttybox_puts(firstbox, print_buffer, OWNER_CLIENT);
	ttybox_putchar(firstbox, '\n', OWNER_CLIENT);


	ttybox_puts(firstbox, "Memory allocation starts at ", OWNER_CLIENT);

	itoa((int)mm_get_start(), print_buffer, 16);
	ttybox_puts(firstbox, print_buffer, OWNER_CLIENT);
	ttybox_putchar(firstbox, '\n', OWNER_CLIENT);


	ttybox_puts(firstbox, "Memory Map starts at ", OWNER_CLIENT);

	itoa(mbi->mmap_addr, print_buffer, 16);
	ttybox_puts(firstbox, print_buffer, OWNER_CLIENT);
	ttybox_putchar(firstbox, '\n', OWNER_CLIENT);
	
	while((int)mmap < mbi->mmap_addr + mbi->mmap_length) {
		ttybox_puts(firstbox, "MM entry number ", OWNER_CLIENT);
		itoa(i, print_buffer, 16);
		ttybox_puts(firstbox, print_buffer, OWNER_CLIENT);
		ttybox_puts(firstbox, " : ", OWNER_CLIENT);
		itoa(mmap->addr, print_buffer, 16);
		ttybox_puts(firstbox, print_buffer, OWNER_CLIENT);
		ttybox_puts(firstbox, " - ", OWNER_CLIENT);
		itoa(mmap->addr + mmap->len, print_buffer, 16);
		ttybox_puts(firstbox, print_buffer, OWNER_CLIENT);
		ttybox_puts(firstbox, " (", OWNER_CLIENT);
		itoa(mmap->len, print_buffer, 16);
		ttybox_puts(firstbox, print_buffer, OWNER_CLIENT);
		ttybox_puts(firstbox, ")\n", OWNER_CLIENT);
		mmap = (multiboot_memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(unsigned int) );
		i++;
	}

	ttybox_display(firstbox);
	ttybox_update_cursor(firstbox);
	
}

