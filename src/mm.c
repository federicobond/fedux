#include "../include/mm.h"
#include "../include/stddef.h"
#include "../include/multiboot.h"
#include "../include/kpanic.h"

#define KM_NORMAL 0

/* TODO: Refactor this better */
extern void *kmain(void);

static void * _memory_start;
static unsigned long int _memory_length;
static void * _next_alloc;

void mm_setup(multiboot_info_t * mbi)
{

	/* 1 MB Safe distance */
	#define SAFE_DISTANCE (1024*1024)
	#define MINIMUM_LEN   (1024*1024)

	unsigned long int largest_len = 0;
	void * largest_addr;
	void * safe_addr = (unsigned long int)kmain + SAFE_DISTANCE + 1;

	multiboot_memory_map_t* mmap = mbi->mmap_addr;

	while(mmap < mbi->mmap_addr + mbi->mmap_length)
	{
		if (mmap->len > largest_len)
		{
			/* Heuristics to determine a ensure a safe distance from kernel code. */
			
			if (mmap->addr > safe_addr) 
			{
				largest_len = mmap->len;
				largest_addr = mmap->addr;
			}
			else if ((safe_addr - mmap->addr) > mmap->len)
			{
				largest_len = mmap->len - ((unsigned long int)safe_addr - mmap->addr);
				largest_addr = safe_addr;
			}
		}

		mmap = (multiboot_memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(unsigned int) );
	}

	
	if (largest_len < MINIMUM_LEN)
		kpanic("mm_setup - Could not find any suitable memory map");


	mm_init(largest_addr, largest_len);

}

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
