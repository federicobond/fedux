#include "../include/ttyman.h"
#include "../include/stdio.h"
#include "../include/serialman.h"

int io_write(int fd, char * data, unsigned int size)
{
	int retval = 0;

	switch (fd)
	{
	case STDIN_FILENO:
		retval = 0;
		break;
	case STDOUT_FILENO:
	case STDERR_FILENO:
		//retval = serialman_write(0, data, size);
		retval = ttyman_write(data, size);
		break;
	}

    return retval;
}

int io_read(int fd, char * data, unsigned int size)
{
	int retval = 0;
	char buffer[64];
	static linear = 0;
	switch (fd)
	{
	case STDIN_FILENO:
		//retval = serialman_read(0, data, size);
		retval = ttyman_read(data, size);
		break;
	case STDOUT_FILENO:
	case STDERR_FILENO:
		retval = 0;
		break;
	}

    return retval;
}
