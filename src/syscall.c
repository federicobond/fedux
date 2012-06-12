#include "../include/syscall.h"
#include "../include/io.h"
#include "../include/ktime.h"
#include "../include/kasm.h"

int syscall_handler(int number,
					int param1,
					int param2,
					int param3,
					int param4,
					int param5)

{
	int retval;

	switch (number)
	{
	case SYSCALL_WRITE:		
		retval = io_write((int)param1, (char*)param2, (unsigned int)param3);
		break;
	case SYSCALL_READ:
		retval = io_read((int)param1, (char*)param2, (unsigned int)param3);
		break;
	case SYSCALL_TIME:
		retval = (int)ktime_mktime();
		break;
	default:
        /* TODO: Print error explanation */
		retval = SYSCALL_ERROR;
		break;
	}

	return retval;
}
