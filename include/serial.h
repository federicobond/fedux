#ifndef _SERIAL_H
#define _SERIAL_H

#define SERIAL_PARITY_N 0
#define SERIAL_PARITY_O 1
#define SERIAL_PARITY_E 2
#define SERIAL_PARITY_M 3
#define SERIAL_PARITY_S 4

void serial_send(int serial_fd, char *data, int size);

int serial_read(int serial_fd, char *data, int size, int timeout_usec);


#endif
