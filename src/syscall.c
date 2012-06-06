

#include "../include/syscall.h"
#include "../include/stdlib.h"
#include "../include/string.h"

int syscall_handler(int number,
					int param1,
					int param2,

					int param3,
					int param4,
					int param5)

{
	int retval;

	char print_buffer[64];

/*
	ttyman_write("SYSCALL ", 9);

	itoa(number, print_buffer, 16);
	ttyman_write(print_buffer, strlen(print_buffer));
	ttyman_write(" - ", 3);

	itoa(param1, print_buffer, 16);
	ttyman_write(print_buffer, strlen(print_buffer));
	ttyman_write(" - ", 3);
	
	itoa(param2, print_buffer, 16);
	ttyman_write(print_buffer, strlen(print_buffer));
	ttyman_write(" - ", 3);
	
	itoa(param3, print_buffer, 16);
	ttyman_write(print_buffer, strlen(print_buffer));
	ttyman_write(" - ", 3);

	itoa(param4, print_buffer, 16);
	ttyman_write(print_buffer, strlen(print_buffer));
	ttyman_write(" - ", 3);

	itoa(param5, print_buffer, 16);
	ttyman_write(print_buffer, strlen(print_buffer));
*/
	switch (number)
	{
	case SYSCALL_WRITE:		
		retval = io_write((int)param1, (char*)param2, (unsigned int)param3);
		break;
	case SYSCALL_READ:
		retval = io_read((int)param1, (char*)param2, (unsigned int)param3);
		break;
	default:
		retval = SYSCALL_ERROR;
		break;
	}

	return retval;
}
