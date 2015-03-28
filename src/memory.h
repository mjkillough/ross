#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdint.h>
#include <stdlib.h>


void* _kmalloc(size_t size, uint8_t aligned);
void* kmalloc(size_t size);
void* kmalloc_aligned(size_t size);

#endif // __MEMORY_H__
