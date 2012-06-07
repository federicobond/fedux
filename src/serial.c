#include "../include/serial.h"
#include "../include/stdio.h"
#include "../include/kpanic.h"

#include "../include/serial.h"





void serial_break_set(char enable)
{
	
}

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
		bq_write_lossless(pd->recv_queue, &datum, sizeof(datum));		
	}

	if (!bq_avail(pd->recv_queue))
			serial_break_set(1);
	
}



int serial_maysend(portdesc_t * pd)
{
	return 1;
}

int serial_mayrecv(portdesc_t * pd)
{
	return 1;
}

int serial_write(portdesc_t * pd, char * data, int size)
{
	serial_flush(pd);
	bq_write(pd->send_queue, data, size);
	serial_flush(pd);
}

int serial_read(portdesc_t * pd, char * data, int size)
{
	serial_flush(pd);
	bq_read(pd->recv_queue, data, size);
	serial_flush(pd);
}


portdesc_t * serial_create(int base_addr)
{
	portdesc_t * pd = (portdesc_t *)mm_malloc(sizeof(portdesc_t));
	
	pd->base_addr = base_addr;
	pd->recv_queue = bq_create(DEFAULT_BUFFSIZE);
	pd->send_queue = bq_create(DEFAULT_BUFFSIZE);
	
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

void
serial_handler()
{
    kpanic("Serial!");
}
