#include "../include/serial.h"

#define PORT_COUNT 4

static portdesc_t * _serial_pds[PORT_COUNT];
static int _serial_base_addr[PORT_COUNT] = { 0x03F8, 0x02F8, 0x03E8, 0x02E8 };
static int _initialized = 0;

int serialman_write(int serialfd, char * data, int size)
{
	int retval = 0;

	if (serialfd >= 0 && serialfd < PORT_COUNT)
	{
		serial_write(_serial_pds[serialfd], data, size);
	}	
}


int serialman_read(int serialfd, char * data, int size)
{
	int retval = 0;

	if (serialfd >= 0 && serialfd < PORT_COUNT)
	{
		serial_read(_serial_pds[serialfd], data, size);
	}	
}

void serialman_init()
{
	int i;
	for (i = 0; i < PORT_COUNT; i++)
		_serial_pds[i] = serial_create(_serial_base_addr[i]);
	_initialized = 1;
}


void serial_handler()
{
	if (_initialized)
	{
		int i;
		for (i = 0; i < PORT_COUNT; i++)
			serial_flush(_serial_pds[i]);
	}
}
