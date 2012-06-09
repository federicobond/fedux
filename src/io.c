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
		retval = ttyman_write(data, size);
		break;
    case TTYS0:
    case TTYS1:
    case TTYS2:
    case TTYS3:
        retval = serialman_write(fd - TTYS0, data, size);
        break;
	}

    return retval;
}

int io_read(int fd, char * data, unsigned int size)
{
	int retval = 0;

	switch (fd)
	{
	case STDIN_FILENO:
		retval = ttyman_read(data, size);
		break;
	case STDOUT_FILENO:
	case STDERR_FILENO:
		retval = 0;
		break;
    case TTYS0:
    case TTYS1:
    case TTYS2:
    case TTYS3:
        retval = serialman_read(fd - TTYS0, data, size);
        break;
	}

    return retval;
}
