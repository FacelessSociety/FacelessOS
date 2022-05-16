#ifndef VMM_H
#define VMM_H

#include <stdint.h>
#include <arch/memory/memory.h>

// 2022 Ian Moffett <ian@kesscoin.com>


typedef enum {
    VMM_P_PRESENT = (1 << 0),
    VMM_RW_WRITABLE = (1 << 1),
    VMM_US_USER = (1 << 2),
} VMM_PAGE_BIT;


void vmm_init(meminfo_t meminfo);
void map_page(uint64_t logical, uint32_t flags);
void unmap_page(uint64_t logical);
void* kalloc_page(uint32_t flags);

#endif
