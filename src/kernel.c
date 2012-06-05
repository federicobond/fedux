#include "../include/kasm.h"
#include "../include/defs.h"
#include "../include/kc.h"
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

void setup_mm(multiboot_info_t * mbi);
void print_memory_map(int linear, multiboot_info_t *mbi);

DESCR_INT idt[0xA];			/* IDT de 10 entradas*/
IDTR idtr;				/* IDTR */

int tickpos = 0;
int cursor = 0;


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
    char *video = (char *) 0xb8000;
    video[tickpos]= '*';
    tickpos += 2;
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

	char datum;

	_cli();

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

/* Initialize memory management */

	setup_mm(mbi);


/* Initialize keyboard driver */
/*	kbd_init(keyboard_callback);*/

        
/* Initialize video driver */

	/*video_init();
	*/

	vgatext_init(80, 25, (char *) 0xB8000);
	vgatext_cursor_setxy(0, 0);
	vgatext_clear();


/* Initialize TTY manager */

	ttyman_init(0, 10, 80, 15);

/* Initialize basic ttybox output */

	firstbox = ttybox_create(0, 10, 80, 15);
	
/*
 ____             __                     
/\  _`\          /\ \                    
\ \ \L\_\  __    \_\ \   __  __   __  _  
 \ \  _\//'__`\  /'_` \ /\ \/\ \ /\ \/'\ 
  \ \ \//\  __/ /\ \L\ \\ \ \_\ \\/>  </ 
   \ \_\\ \____\\ \___,_\\ \____/ /\_/\_\
    \/_/ \/____/ \/__,_ / \/___/  \//\/_/


*/

	vgatext_format_set(0x70);
	vgatext_charfill(0, 0, 80, 1, (char)205);
	vgatext_charfill(0, 8, 80, 1, (char)205);
	vgatext_charfill(0, 0, 1, 8, (char)186);
	vgatext_charfill(79, 0, 1, 8, (char)186);

	vgatext_charfill(79, 0, 1, 1, (char)187);
	vgatext_charfill(0, 8, 1, 1, (char)200);
	vgatext_charfill(0, 0, 1, 1, (char)201);
	vgatext_charfill(79, 8, 1, 1, (char)188);

	vgatext_format_set(0x9F);
	vgatext_charfill(1, 1, 78, 7, 0);
	vgatext_print(vgatext_poslinear(1, 1), " ____             __ ");
	vgatext_print(vgatext_poslinear(1, 2), "/\\  _`\\          /\\ \\                    ");
	vgatext_print(vgatext_poslinear(1, 3), "\\ \\ \\L\\_\\  __    \\_\\ \\   __  __   __  _  ");
	vgatext_print(vgatext_poslinear(1, 4), " \\ \\  _\\//'__`\\  /'_` \\ /\\ \\/\\ \\ /\\ \\/'\\ ");
	vgatext_print(vgatext_poslinear(1, 5), "  \\ \\ \\//\\  __/ /\\ \\L\\ \\\\ \\ \\_\\ \\\\/>  </ ");
	vgatext_print(vgatext_poslinear(1, 6), "   \\ \\_\\\\ \\____\\\\ \\___,_\\\\ \\____/ /\\_/\\_\\");
	vgatext_print(vgatext_poslinear(1, 7), "    \\/_/ \\/____/ \\/__,_ / \\/___/  \\//\\/_/     Welcome to Fedux Kernel 0.0.1!");
	vgatext_format_set(0x0F);
	
	
	

/* End of critical initializations: Re-enable interrupts */
	_sti();

	print_memory_map(vgatext_poslinear(0, 9),mbi);

	_hlt();

	while(1)
	{
		ttyman_write("root # ", 7);
		datum = 0;
		while (datum != '\n')
			ttyman_read(&datum, sizeof(char));
	}

	return 0;
	
}


void setup_mm(multiboot_info_t * mbi)
{

	/* 1 MB Safe distance */
	#define SAFE_DISTANCE (1024*1024)
	#define MINIMUM_LEN   (1024*1024)

	unsigned long int largest_len = 0;
	void * largest_addr;
	void * safe_addr = (unsigned long int)kmain + SAFE_DISTANCE + 1;

	multiboot_memory_map_t* mmap = mbi->mmap_addr;

	while(mmap < mbi->mmap_addr + mbi->mmap_length)
	{
		if (mmap->len > largest_len)
		{
			/* Heuristics to determine a ensure a safe distance from kernel code. */
			
			if (mmap->addr > safe_addr) 
			{
				largest_len = mmap->len;
				largest_addr = mmap->addr;
			}
			else if ((safe_addr - mmap->addr) > mmap->len)
			{
				largest_len = mmap->len - ((unsigned long int)safe_addr - mmap->addr);
				largest_addr = safe_addr;
			}
		}

		mmap = (multiboot_memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(unsigned int) );
	}

	
	if (largest_len < MINIMUM_LEN)
		kpanic("setup_mm - Could not find any suitable memory map");


	mm_init(largest_addr, largest_len);

}


void print_memory_map(int linear, multiboot_info_t *mbi)
{
	int i = 0;
	char print_buffer[64];	
	multiboot_memory_map_t *mmap = (multiboot_memory_map_t *)mbi->mmap_addr;

	ttybox_puts(firstbox, "kmain starts at ");

	itoa((int)kmain, print_buffer, 16);
	ttybox_puts(firstbox, print_buffer);
	ttybox_putchar(firstbox, '\n');


	ttybox_puts(firstbox, "Memory allocation starts at ");

	itoa((int)mm_get_start(), print_buffer, 16);
	ttybox_puts(firstbox, print_buffer);
	ttybox_putchar(firstbox, '\n');


	ttybox_puts(firstbox, "Memory Map starts at ");

	itoa(mbi->mmap_addr, print_buffer, 16);
	ttybox_puts(firstbox, print_buffer);
	ttybox_putchar(firstbox, '\n');
	
	while((int)mmap < mbi->mmap_addr + mbi->mmap_length) {
		ttybox_puts(firstbox, "MM entry number ");
		itoa(i, print_buffer, 16);
		ttybox_puts(firstbox, print_buffer);
		ttybox_puts(firstbox, " : ");
		itoa(mmap->addr, print_buffer, 16);
		ttybox_puts(firstbox, print_buffer);
		ttybox_puts(firstbox, " - ");
		itoa(mmap->addr + mmap->len, print_buffer, 16);
		ttybox_puts(firstbox, print_buffer);
		ttybox_puts(firstbox, " (");
		itoa(mmap->len, print_buffer, 16);
		ttybox_puts(firstbox, print_buffer);
		ttybox_puts(firstbox, ")\n");
		mmap = (multiboot_memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(unsigned int) );
		i++;
	}

	ttybox_display(firstbox);
	ttybox_update_cursor(firstbox);
	
}

