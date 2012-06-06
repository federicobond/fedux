#include "../include/serial.h"
#include "../include/stdio.h"
#include "../include/kpanic.h"

typedef enum { 
    B110 = 0x00,
    B150 = 0x20,
    B300 = 0x40,
    B600 = 0x60,
    B1200 = 0x80,
    B2400 = 0xA0,
    B4800 = 0xC0,
    B9600 = 0xE0
} baud_t;

typedef enum {
    CS7 = 0x02,
    CS8 = 0x03
} data_bits_t;

typedef enum {
    STOPB1 = 0x00,
    STOPB2 = 0x04
} stop_bits_t;

typedef enum {
    PARNO = 0x00,
    PARODD = 0x08,
    PAREVEN = 0x18
} parity_t;

baud_t      _baud;
data_bits_t _data_bits;
stop_bits_t _stop_bits;
parity_t    _parity;

void
serial_setup(baud_t baud, data_bits_t data_bits, stop_bits_t stop_bits, parity_t parity)
{
    _baud = baud;
    _data_bits = data_bits;
    _parity = parity;
}

void
serial_send(int serial_fd, char *data, int size)
{
    write(serial_fd, data, size);
    
}

int
serial_read(int serial_fd, char *data, int size, int timeout_usec)
{
    read(serial_fd, data, size);
}

void
serial_handler()
{
    kpanic("Serial!");
}
