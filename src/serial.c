#include "../include/serial.h"
#include "../include/stdio.h"
#include "../include/kpanic.h"

#include "../include/serial.h"






void serial_flush(portdesc_t * pd)
{
	char datum;

	while (serial_maysend(pd) && bq_used(pd->send_queue))
	{
		bq_read(pd->send_queue, &datum, sizeof(datum));

		_outb(datum, pd->base_addr + SERIAL_THR);
	}

	while (serial_mayrecv(pd) && bq_avail(pd->recv_queue))
	{
		datum = _inb(pd->base_addr + SERIAL_RBR);
		if (datum == 0x0D) datum = 0x0A;
		bq_write_lossless(pd->recv_queue, &datum, sizeof(datum));	
	}

	if (!bq_avail(pd->recv_queue))
			serial_break_set(1);
	
}


void serial_requestsend(portdesc_t * pd)
{
	int mcr = _inb(pd->base_addr + SERIAL_MCR);
}

int serial_maysend(portdesc_t * pd)
{
	char lsr = _inb(pd->base_addr + SERIAL_LSR);
	char msr = _inb(pd->base_addr + SERIAL_MSR);
	
	return ((lsr & LSR_THR) && 
			(msr & MSR_CLEAR_SEND) &&
			(msr & MSR_CARRIER_DETECT) &&
			(msr & MSR_DATA_SET_READY));
}

int serial_mayrecv(portdesc_t * pd)
{
	char lsr = _inb(pd->base_addr + SERIAL_LSR);
	char msr = _inb(pd->base_addr + SERIAL_MSR);

	return ((lsr & LSR_DATA_AVAIL) && 
			(msr & MSR_CLEAR_SEND) &&
			(msr & MSR_CARRIER_DETECT) &&
			(msr & MSR_DATA_SET_READY));
}


void serial_baud_set(portdesc_t * pd, baud_t baud)
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

void serial_lcr_setup(portdesc_t * pd,
					  data_bits_t data_bits, stop_bits_t stop_bits,
					  parity_t parity)
{
	pd->data_bits = data_bits;
	pd->stop_bits = stop_bits;
	pd->parity = parity;

	char lcr = _inb(pd->base_addr + SERIAL_LCR);

	lcr &= ~LCR_SETUP_MASK;
	lcr |= pd->data_bits;
	lcr |= pd->stop_bits;
	lcr |= pd->parity;	

	_outb(lcr, pd->base_addr + SERIAL_LCR);

}

void serial_ier_setup(portdesc_t * pd)
{
	char ier = _inb(pd->base_addr + SERIAL_IER);
	ier |= IER_DEFAULT;
	_outb(ier, pd->base_addr + SERIAL_IER);
}


void serial_break_set(portdesc_t * pd, char enable)
{
	char lcr = _inb(pd->base_addr + SERIAL_LCR);

	
}


void serial_dtr(portdesc_t * pd)
{
	char mcr = _inb(pd->base_addr + SERIAL_MCR);
	mcr |= MCR_DATA_TERM;
	_outb(mcr, pd->base_addr + SERIAL_MCR);
}

int serial_write(portdesc_t * pd, char * data, int size)
{
	int retval;
	serial_flush(pd);
	retval = bq_write(pd->send_queue, data, size);
	serial_flush(pd);
	return retval;
}

int serial_read(portdesc_t * pd, char * data, int size)
{
	int retval;
	serial_flush(pd);
	retval = bq_read(pd->recv_queue, data, size);
	serial_flush(pd);
	return retval;
}


portdesc_t * serial_create(int base_addr)
{
	portdesc_t * pd = (portdesc_t *)mm_malloc(sizeof(portdesc_t));
	
	pd->base_addr = base_addr;
	pd->recv_queue = bq_create(DEFAULT_BUFFSIZE);
	pd->send_queue = bq_create(DEFAULT_BUFFSIZE);

	serial_dtr(pd);
	serial_baud_set(pd, B9600);
	serial_lcr_setup(pd, CS8, STOPB1, PAREVEN);	
	serial_ier_setup(pd);

	return pd;
}

void serial_destroy(portdesc_t * pd)
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


