#ifndef  _KERNEL_MEMORY_H
#define  _KERNEL_MEMORY_H
#include "stdint.h"
#include "bitmap.h"

struct virtual_addr {
    struct bitmap vaddr_bitmap;
    uint32_t vaddr_start;
};

static void mem_pool_init (uint32_t all_mem);
void mem_init ();

#endif
