#ifndef _BQ_H
#define _BQ_H

typedef struct byte_queue
{
	unsigned int size;
	char * buff;
	unsigned int write;
	unsigned int read;
	bool full;
	char lossless;

} byte_queue;


void bq_init(byte_queue * queue, char * buff, unsigned int size);
int bq_used(byte_queue * queue);

int bq_write(byte_queue * queue, const char * data, unsigned int size);
int bq_write_lossless(byte_queue * queue, const char * data, unsigned int size);

int bq_read(byte_queue * queue, char * data, unsigned int size);
int bq_peek(byte_queue * queue, char * data, unsigned int size);



#endif
