



#include "../include/string.h"
#include "../include/vgatext.h"
#include "../include/bq.h"

#define MAXBUFFERSIZE

byte_queue _input_queue;
byte_queue _output_queue;

/* TODO: Dynamically alloc buffers */

char _input_buffer[MAXBUFFERSIZE];
char _output_buffer[MAXBUFFERSIZE];


int tty_input_write(const char * data, int size)
{
	return bq_write(_input_queue, data, size)
}


int tty_input_read(char * data, int size)
{
	return bq_read(_input_queue, data, size);
}

int tty_output_write(const char * data, int size)
{
	return bq_write(_output_queue, data, size);
}
