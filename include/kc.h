/* TODO: What do we do with this file? */
/**********************
 kc.h
**********************/
#include "defs.h"

#ifndef _KC_H
#define _KC_H

/* Inicializa la entrada del IDT */
void setup_idt_entry(DESCR_INT *item, byte selector, dword offset, byte access,
			 byte cero);

#endif
