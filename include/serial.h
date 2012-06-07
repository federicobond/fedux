#ifndef _SERIAL_H
#define _SERIAL_H

#include "../include/bq.h"

#define SERIAL_PARITY_N 0
#define SERIAL_PARITY_O 1
#define SERIAL_PARITY_E 2
#define SERIAL_PARITY_M 3
#define SERIAL_PARITY_S 4

#define SERIAL_BASE_ADDR 0
#define SERIAL_RBR	SERIAL_BASE_ADDR
#define SERIAL_THR	SERIAL_BASE_ADDR

#define SERIAL_IER	SERIAL_BASE_ADDR+1

#define SERIAL_DLL	SERIAL_BASE_ADDR
#define SERIAL_DLM	SERIAL_BASE_ADDR+1

#define SERIAL_IIR	SERIAL_BASE_ADDR+2
#define SERIAL_FCR	SERIAL_BASE_ADDR+2

#define SERIAL_LCR	SERIAL_BASE_ADDR+3
#define SERIAL_MCR	SERIAL_BASE_ADDR+4

#define SERIAL_LSR	SERIAL_BASE_ADDR+5
#define SERIAL_MSR	SERIAL_BASE_ADDR+6

#define SERIAL_SCR	SERIAL_BASE_ADDR+7

#define DLAB_MASK	0x80

#define FIFO_ENABLE_MASK (1 << 3)
#define FIFO_TLEVEL_MASK (3 << 6)

#define FIFO_DMA_MASK (1 << 3)

#define PARITY_MASK (7 << 3)
#define DWL_MASK (15 << 0)

#define STOPBIT_MASK (1 << 2)

#define BREAK_MASK (1 << 6)

#define DEFAULT_BUFFSIZE 512


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


typedef struct portdesc_t
{
	baud_t baud;
	data_bits_t data_bits;
	stop_bits_t stop_bits;
	parity_t parity;
	int base_addr;
	byte_queue * send_queue;
	byte_queue * recv_queue;

} portdesc_t;


int serial_write(portdesc_t * pd, char * data, int size);
int serial_read(portdesc_t * pd, char * data, int size);

portdesc_t * serial_create(int base_addr);


#endif
