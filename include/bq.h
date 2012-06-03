#ifndef _BQ_H
#define _BQ_H

#include "../include/stddef.h"

typedef struct byte_queue
{
	unsigned int size;
	char * buff;
	unsigned int write;
	unsigned int read;
	bool full;
	char lossless;

} byte_queue;


/*	bq_init
	Initializes a queue.
	Data buffer must be provided as it cannot be dynamically alloc'd.
*/

void bq_init(byte_queue *queue, char *buff, unsigned int size);


/*	bq_used
	Returns number of bytes used in the queue.
*/

int bq_used(byte_queue *queue);

/*	bq_get_write
	Returns write index position.
*/
int bq_get_write(byte_queue *queue);

/*	bq_get_read
	Returns read index position.
*/
int bq_get_read(byte_queue *queue);

/*	bq_write
	Writes data to queue.
	If the queue does not have enough space, the oldest
	bytes are removed.
*/

int bq_write(byte_queue *queue, const char *data, unsigned int size);

/*	bq_write_lossless
	Writes data to queue.
	If the queue does not have enough space available, no data
	is written.
*/

int bq_write_lossless(byte_queue *queue, const char *data, unsigned int size);


/* 	bq_read
	Reads oldest bytes from queue and then removes them.
*/

int bq_read(byte_queue *queue, char *data, unsigned int size);

/* 	bq_peek
	Reads oldest bytes from queue without removing them.
*/
int bq_peek(byte_queue *queue, char *data, unsigned int size);



int bq_rread(byte_queue *queue, char *data, unsigned int size);


int bq_rpeek(byte_queue *queue, char *data, unsigned int size);

#endif
