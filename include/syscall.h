

#ifndef _SYSCALL_H
#define _SYSCALL_H

#define SYSCALL_WRITE 	0
#define SYSCALL_READ	1

#define SYSCALL_TIME	2

#define SYSCALL_ERROR -1

int syscall_handler(int number,
					int param1,
					int param2,
					int param3,
					int param4,
					int param5);



#endif


