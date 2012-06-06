#ifndef _MM_H
#define _MM_H

#include "stddef.h"
#include "multiboot.h"

void mm_setup(multiboot_info_t * mbi);
void mm_init(void * memory_start, unsigned long int length);
void mm_free(void * ptr);
void * mm_malloc(size_t size);
void * mm_get_start();

#endif
