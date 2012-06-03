
#include "../include/stddef.h"

#define KM_NORMAL 0

static void * _memory_start;
static unsigned long int _memory_length;
static void * _next_alloc;



void mm_init(void * memory_start, unsigned long int memory_length)
{
	_memory_start = memory_start;
	_memory_length = memory_length;
	_next_alloc = memory_start;
}


void * mm_get_start()
{
	return _memory_start;
}

void mm_free(void * ptr)
{
	/* Do nothing! */
}


void * mm_malloc(size_t size)
{
	_next_alloc = ((char *)_next_alloc) + size;

	if (_next_alloc - _memory_start > _memory_length)
		kpanic("@kmalloc - Out of memory");

	return (((char *)_next_alloc) - size);
}
