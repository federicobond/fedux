#include "../include/serial.h"
#include "../include/critical.h"

#define PORT_COUNT 1

static portdesc_t * _serial_pds[PORT_COUNT];
//static int _serial_base_addr[PORT_COUNT] = { 0x03F8, 0x02F8, 0x03E8, 0x02E8 };
static int _serial_base_addr[PORT_COUNT] = { 0x03F8 };
static int _initialized = 0;

int
serialman_write(int serialfd, char * data, int size)
{
	int retval = 0;

	critical_enter();

	if (serialfd >= 0 && serialfd < PORT_COUNT)
	{
		retval = serial_write(_serial_pds[serialfd], data, size);
	}

	critical_leave();

	return retval;
}


int
serialman_read(int serialfd, char * data, int size)
{
	int retval = 0;

	critical_enter();

	if (serialfd >= 0 && serialfd < PORT_COUNT)
	{
		retval = serial_read(_serial_pds[serialfd], data, size);
	}	

	critical_leave();

	return retval;
}

void
serialman_init()
{
	int i;
	for (i = 0; i < PORT_COUNT; i++)
		_serial_pds[i] = serial_create(_serial_base_addr[i]);
	_initialized = 1;
}


void
serial_handler()
{
#ifdef DEBUG_SERIAL
	ttyman_write("I",1);
#endif
	critical_enter();
	if (_initialized)
	{
		int i;
		for (i = 0; i < PORT_COUNT; i++)
		{
			serial_flush(_serial_pds[i]);
			serial_interrupt_reset(_serial_pds[i]);			
		}	
	}
	critical_leave();
}

