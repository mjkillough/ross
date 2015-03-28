#include <stdint.h>
#include <stdlib.h>

#include "memory.h"


// If aligned=1, this returns a 16KB aligned chunk of memory. (We need this
// for 1st level page tables, but I don't know what alignment we'll need
// elsewhere).
// This is currently a very very naive implementation of kmalloc, which we
// can revisit later.
static void* _free_base = (void*)0x400001;
void* _kmalloc(size_t size, uint8_t aligned)
{
    if (aligned && ((uint32_t)_free_base & 0x2FFF) != 0) {
        _free_base = (void*)(((uint32_t)_free_base + 0x4000) & 0xFFFFC000);
    }
    void* ret = _free_base;
    _free_base += size;
    return ret;
}

void* kmalloc(size_t size)
{
    return _kmalloc(size, 0);
}

void* kmalloc_aligned(size_t size)
{
    return _kmalloc(size, 1);
}

