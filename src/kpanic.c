
#include "../include/kpanic.h"
#include "../include/vgatext.h"
#include "../include/kasm.h"


void kpanic(void * panic_info)
{
	char * panic_msg = (char *) panic_info;
	int linear = 0;

	_cli();

	vgatext_init(80, 25, (char *) 0xB8000);
	vgatext_format_set(0x4F);
	vgatext_charfill(0, 0, 80, 25, 0);


	linear = vgatext_print(linear, "Run for your life!");
	linear += 80 - (linear % 80);
	linear = vgatext_print(linear, "Panic: ");
	linear = vgatext_print(linear, panic_msg);

	/* Halt forever! */

	while (1)
		_hlt();

}

