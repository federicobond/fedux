/*********************************************
kasm.h

************************************************/

#include "defs.h"

void		_outb(char value, char *port);

unsigned int    _read_msw();

void            _lidt(IDTR *idtr);

void		_mask_pic_1(byte mascara);  /* Escribe mascara de PIC1 */
void		_mask_pic_2(byte mascara);  /* Escribe mascara de PIC2 */

void		_cli(void);        /* Deshabilita interrupciones  */
void		_sti(void);	 /* Habilita interrupciones  */

void		_timertick_handler();      /* Timer tick */

void		_debug (void);

