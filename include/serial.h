#ifndef _SERIAL_H
#define _SERIAL_H

#include "../include/bq.h"

#define SERIAL_PARITY_N 0
#define SERIAL_PARITY_O 1
#define SERIAL_PARITY_E 2
#define SERIAL_PARITY_M 3
#define SERIAL_PARITY_S 4

#define SERIAL_RBR	0
#define SERIAL_THR	0

#define SERIAL_IER	1

#define SERIAL_DLL	0
#define SERIAL_DLM	1

#define SERIAL_IIR	2
#define SERIAL_FCR	2

#define SERIAL_LCR	3
#define SERIAL_MCR	4

#define SERIAL_LSR	5
#define SERIAL_MSR	6

#define SERIAL_SCR	7

#define LCR_DLAB_MASK	(1 << 7)
#define LCR_BREAK_MASK  (1 << 6)

#define FIFO_ENABLE_MASK (1 << 3)
#define FIFO_TLEVEL_MASK (3 << 6)

#define FIFO_DMA_MASK (1 << 3)

#define PARITY_MASK (7 << 3)
#define DWL_MASK (15 << 0)

#define STOPBIT_MASK (1 << 2)

#define BREAK_MASK (1 << 6)

#define DEFAULT_BUFFSIZE 512

#define LCR_SETUP_MASK 0x3F

#define MSR_CLEAR_SEND (1 << 4)
#define MSR_DATA_SET_READY (1 << 5)
#define MSR_RING_INDICATOR (1 << 6)
#define MSR_CARRIER_DETECT (1 << 7)

#define LSR_DATA_AVAIL 		(1)
#define LSR_OVERRUN	   (1 << 1)
#define LSR_PARITY	   (1 << 2)
#define LSR_FRAMING	   (1 << 3)
#define LSR_BREAK	   (1 << 4)
#define LSR_THR		   (1 << 5)
#define LSR_THR_LINE   (1 << 6)
#define LSR_FIFOERR	   (1 << 7)

#define MCR_DATA_TERM  (1 << 0)
#define MCR_SEND_REQ   (1 << 1)

#define IER_DATA_AVAIL (1 << 0)
#define IER_THR		   (1 << 1)
#define IER_LSR		   (1 << 2)
#define IER_MSR		   (1 << 3)

#define IER_DEFAULT (IER_DATA_AVAIL | IER_THR | IER_LSR | IER_MSR)
#define MCR_DEFAULT 0x0B
#define FCR_DEFAULT 0x07

typedef enum { 
    B4800 = 0x18,
    B9600 = 0x0C
} baud_t;

typedef enum {
	CS5 = 0x00,
	CS6 = 0x01,
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
void serial_interrupt_reset(portdesc_t * pd);
void serial_flush(portdesc_t * pd);

portdesc_t * serial_create(int base_addr);


#endif
