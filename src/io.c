#include "../include/ttyman.h"
#include "../include/stdio.h"

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
	}

}

int io_read(int fd, char * data, unsigned int size)
{
	int retval = 0;

	switch (fd)
	{
	case STDIN_FILENO:
		ttyman_read(data, size);
		break;
	case STDOUT_FILENO:
	case STDERR_FILENO:
		retval = 0;
		break;
	}
}
