

#define SYSCALL_WRITE 	0
#define SYSCALL_READ	1

#define SYSCALL_ERROR -1

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
		break;
		retval = io_write((int)param1, (char*)param2, (unsigned int)param3);
	case SYSCALL_READ:
		retval = io_read((int)param1, (char*)param2, (unsigned int)param3);
		break;
	default:
		retval = SYSCALL_ERROR;
		break;
	}

	return retval;
}
