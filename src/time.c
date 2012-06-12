#include "../include/time.h"
#include "../include/syscall.h"
#include "../include/kasm.h"

time_t
time(time_t *time)
{
	return ((time_t)_syscall(SYSCALL_TIME, 0, 0, 0, 0, 0));
}


