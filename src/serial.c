#include "../include/serial.h"
#include "../include/stdio.h"
#include "../include/kpanic.h"
#include "../include/kasm.h"
#include "../include/serial.h"
#include "../include/mm.h"

#include "../include/ttyman.h"



int serial_maysend(portdesc_t * pd);
int serial_mayrecv(portdesc_t * pd);
void serial_interrupt_reset(portdesc_t * pd);
void serial_flush(portdesc_t * pd);
void serial_baud_set(portdesc_t * pd, baud_t baud);
void serial_lcr_setup(portdesc_t * pd,
					  data_bits_t data_bits, stop_bits_t stop_bits,
					  parity_t parity,
					  break_signal_t break_signal);
void serial_ier_setup(portdesc_t * pd);
void serial_mcr_setup(portdesc_t * pd);
void serial_fcr_setup(portdesc_t * pd);



void
serial_interrupt_reset(portdesc_t * pd)
{
#ifdef DEBUG_SERIAL
	ttyman_write("(R)", 3);
#endif
	_inb(pd->base_addr + SERIAL_IIR);
	_inb(pd->base_addr + SERIAL_MSR);
	_inb(pd->base_addr + SERIAL_LSR);
}

void
serial_flush(portdesc_t * pd)
{
	char datum;

#ifdef DEBUG_SERIAL
	ttyman_write("F",1);
#endif

	while (serial_maysend(pd) && bq_used(pd->send_queue))
	{

#ifdef DEBUG_SERIAL
		ttyman_write("W",1);
#endif
		bq_read(pd->send_queue, &datum, sizeof(datum));
		_outb(datum, pd->base_addr + SERIAL_THR);
	}


	while (serial_mayrecv(pd) && bq_avail(pd->recv_queue))
	{
#ifdef DEBUG_SERIAL
		ttyman_write("R",1);
#endif
		datum = _inb(pd->base_addr + SERIAL_RBR);
		if (datum == 0x0D) datum = 0x0A;
		bq_write_lossless(pd->recv_queue, &datum, sizeof(datum));
	}
	
}

int
serial_maysend(portdesc_t * pd)
{
	char lsr = _inb(pd->base_addr + SERIAL_LSR);
	return (lsr & LSR_THR);
}

int
serial_mayrecv(portdesc_t * pd)
{
	char lsr = _inb(pd->base_addr + SERIAL_LSR);
	return (lsr & LSR_DATA_AVAIL);
}


void
serial_baud_set(portdesc_t * pd, baud_t baud)
{
	char lcr = _inb(pd->base_addr + SERIAL_LCR);
	
	lcr |= LCR_DLAB_MASK; /* DLAB = 1 */
	_outb(lcr, pd->base_addr + SERIAL_LCR);

	pd->baud = baud;
	_outb(pd->baud, pd->base_addr + SERIAL_DLL);
	_outb(pd->baud >> 8, pd->base_addr + SERIAL_DLM); 

	lcr &= ~LCR_DLAB_MASK; /* DLAB = 0 */
	_outb(lcr, pd->base_addr + SERIAL_LCR);
	
}

void
serial_lcr_setup(portdesc_t * pd,
					  data_bits_t data_bits, stop_bits_t stop_bits,
					  parity_t parity,
					  break_signal_t break_signal)
{
	pd->data_bits = data_bits;
	pd->stop_bits = stop_bits;
	pd->parity = parity;
	pd->break_signal = break_signal;

	_outb(pd->data_bits |
		  pd->stop_bits |
		  pd->parity	|
		  pd->break_signal,
		  pd->base_addr + SERIAL_LCR);

}


void
serial_ier_setup(portdesc_t * pd)
{
	_outb(IER_DEFAULT, pd->base_addr + SERIAL_IER);
}

void
serial_mcr_setup(portdesc_t * pd)
{
	_outb(MCR_DEFAULT, pd->base_addr + SERIAL_MCR);
}

void
serial_fcr_setup(portdesc_t * pd)
{
	_outb(FCR_DEFAULT, pd->base_addr + SERIAL_FCR);
}

int
serial_write(portdesc_t * pd, char * data, int size)
{
	int retval;
	//serial_flush(pd);
	retval = bq_write(pd->send_queue, data, size);
	serial_flush(pd);
	return retval;
}

int
serial_read(portdesc_t * pd, char * data, int size)
{
	int retval;
	//serial_flush(pd);
	retval = bq_read(pd->recv_queue, data, size);
	//serial_flush(pd);
	return retval;
}


portdesc_t *
serial_create(int base_addr)
{
	portdesc_t * pd = (portdesc_t *)mm_malloc(sizeof(portdesc_t));
	
	pd->base_addr = base_addr;
	pd->recv_queue = bq_create(DEFAULT_BUFFSIZE);
	pd->send_queue = bq_create(DEFAULT_BUFFSIZE);

	serial_mcr_setup(pd);
	serial_baud_set(pd, B9600);
	serial_lcr_setup(pd, CS8, STOPB1, PAREVEN, NOBREAKSIGNAL);	
	serial_ier_setup(pd);
	serial_fcr_setup(pd);


	serial_interrupt_reset(pd);

	serial_flush(pd);

	/* FIFO CTRL REG */
	//_outb(pd->base_addr + 2, 0xC7);


	return pd;
}

void
serial_destroy(portdesc_t * pd)
{
	mm_free(pd);
}

void port_set_bitrange(int port, char datum, char mask)
{
	char value = _inb(port);
	char pos_track = mask;

	while ((pos_track % 2) == 0)
	{
		datum = datum << 1;
		pos_track = pos_track >> 1;	
	}

	value &= ~mask;
	value |= (datum & mask);

	_outb(value, port);
}

