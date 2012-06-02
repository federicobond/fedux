#include "../include/stddef.h"
#include "../include/bq.h"


void bq_init(byte_queue *queue, char *buff, unsigned int size)
{
	queue->buff = buff;
	queue->size = size;
	queue->write = 0;
	queue->read = 0;
	queue->full = false;
}

int bq_used(byte_queue *queue)
{
	int used;

	if (queue->write > queue->read)
		used = queue->write - queue->read;
	else if (queue->write < queue->read)
		used = queue->size - (queue->read - queue->write);
	else if (queue->full)
		used = queue->size;
	else
		used = 0;

	return used;
}

int bq_write_lossless(byte_queue *queue, const char *data, unsigned int size)
{
	int i, written = 0;

	if (size <= queue->size - bq_used(queue))
	{
		for (i = 0; i < size; i++)
			queue->buff[(i + queue->write) % queue->size] = data[i];
		
		queue->write = (queue->write + size) % queue->size;

		if (queue->write == queue->read)
			queue->full = true;

		written = size;
	}
	

	return written;
}

int bq_write(byte_queue *queue, const char *data, unsigned int size)
{
	int i;

	for (i = 0; i < size; i++)
		queue->buff[(i + queue->write) % queue->size] = data[i];
	
	if (size + bq_used(queue) >= queue->size)
	{
		queue->full = true;
		queue->write = (queue->write + size) % queue->size;
		queue->read = queue->write;
	}
	else
	{
		queue->write = (queue->write + size) % queue->size;
	}

	return size;
}

int bq_read(byte_queue *queue, char *data, unsigned int size)
{
	int read;

	read = bq_peek(queue, data, size);

	if (read)
	{
		queue->full = false;
		queue->read = (queue->read + size) % queue->size;
	}

	return read; 

}


int bq_peek(byte_queue *queue, char *data, unsigned int size)
{
	int i;

	if (size > bq_used(queue))
		size = bq_used(queue);

	
	if (data != NULL)
		for (i = 0; i < size; i++)
			data[i] = queue->buff[(i + queue->read) % queue->size];

	return size;
}

int bq_get_write(byte_queue *queue)
{
	return queue->write;
}

int bq_get_read(byte_queue *queue)
{
	return queue->read;
}

int bq_pop(byte_queue *queue, char *data, unsigned int size)
{
	
}

int bq_rpeek(byte_queue *queue, char *data, unsigned int size)
{
	
}
