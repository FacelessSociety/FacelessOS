#include <arch/memory/vmm.h>
#include <debug/log.h>
#include <interrupts/exceptions.h>
#include <util/mem.h>
#include <stddef.h>

// 2022 Ian Moffett <ian@kesscoin.com>

#define PAGE_ADDR_MASK 0x000ffffffffff000
#define GB 0x40000000UL
#define KERNEL_RESERVED_START 0x0000000000008000
#define KERNEL_RESERVED_END 0xC4B4


__attribute__((aligned(0x1000))) static struct MappingTable {
    uint64_t entries[512];
} pml4;


static uint64_t placement = 0xDEADBEEF;
static uint64_t pages_left = 0;


static void set_placement(meminfo_t meminfo) {
    uint64_t mmap_entries = get_mmap_entries(meminfo);

    memdesc_t* biggest_entry = NULL;

    for (uint64_t entry = 0; entry < mmap_entries; ++entry) {
        memdesc_t* current = mmap_iterator_helper(entry, meminfo);
       
        // If type is usable memory and biggest_entry is NULL, set the biggest_entry to be the segment.
        if (current->type == MMAP_USABLE_MEMORY && biggest_entry == NULL) {
            biggest_entry = current;
            continue;
        }

        // If current has more pages than the biggest entry, re-assign the biggest entry.
        if (current->type == MMAP_USABLE_MEMORY && current->nPages > biggest_entry->nPages)
            biggest_entry = current;
    }

    // If it stays NULL something went wrong.
    if (biggest_entry == NULL)
        panic("NO MEMORY LEFT!\n");

    // Set our placement and amount of pages we have left.
    placement = (uint64_t)biggest_entry->physAddr;
    pages_left = biggest_entry->nPages;
}


static inline uint64_t alloc_frame_internal(void) {
    uint64_t addr = placement;

    if (pages_left == 0)
        panic("NO MEMORY LEFT!\n");

    placement += PAGE_SIZE;
    --pages_left;
    return addr;
}


void map_page(uint64_t logical, uint32_t flags) {
    // Grab indices from logical address.
    uint64_t pml4_idx = (logical >> 39) & 0x1FF;
    uint64_t pdpt_idx = (logical >> 30) & 0x1FF;
    uint64_t pd_idx = (logical >> 21) & 0x1FF;
    uint64_t pt_idx = (logical >> 12) & 0x1FF;

    flags |= VMM_P_PRESENT;         // Ensure it is present.

    // There is no PDPT defined at this index yet.
    if (!(pml4.entries[pml4_idx] & VMM_P_PRESENT)) {
        uint64_t pdpt_frame = alloc_frame_internal();           // Allocate a frame.
        memzero((void*)pdpt_frame, PAGE_SIZE);                  // Zero it.
        pml4.entries[pml4_idx] = (pdpt_frame & PAGE_ADDR_MASK) | flags;
        map_page(pdpt_frame, flags);
    }

    struct MappingTable* pdpt = (struct MappingTable*)(pml4.entries[pml4_idx] & PAGE_ADDR_MASK);

    // No page directory at this index.
    if (!(pdpt->entries[pdpt_idx] & VMM_P_PRESENT)) {
        uint64_t pd_frame = alloc_frame_internal();
        memzero((void*)pd_frame, PAGE_SIZE);
        pdpt->entries[pdpt_idx] = (pd_frame & PAGE_ADDR_MASK) | flags;
        map_page(pd_frame, flags);
    }

    struct MappingTable* pd = (struct MappingTable*)(pdpt->entries[pdpt_idx] & PAGE_ADDR_MASK);

    // No page table for this index.
    if (!(pd->entries[pd_idx] & VMM_P_PRESENT)) {
        uint64_t pt_frame = alloc_frame_internal();
        memzero((void*)pt_frame, PAGE_SIZE);
        pd->entries[pd_idx] = (pt_frame & PAGE_ADDR_MASK) | flags;
        map_page(pt_frame, flags);
    }

    struct MappingTable* pt = (struct MappingTable*)(pd->entries[pd_idx] & PAGE_ADDR_MASK);

    // No page defined for this index.
    if (!(pt->entries[pt_idx] & VMM_P_PRESENT))
        pt->entries[pt_idx] = (logical & PAGE_ADDR_MASK) | flags;
}


void unmap_page(uint64_t logical) {
    if (logical % 0x1000 != 0) return;

    // Grab indices from logical address.
    uint64_t pml4_idx = (logical >> 39) & 0x1FF;
    uint64_t pdpt_idx = (logical >> 30) & 0x1FF;
    uint64_t pd_idx = (logical >> 21) & 0x1FF;
    uint64_t pt_idx = (logical >> 12) & 0x1FF;

    struct MappingTable* pdpt = (struct MappingTable*)(pml4.entries[pml4_idx] & PAGE_ADDR_MASK);
    struct MappingTable* pd = (struct MappingTable*)(pdpt->entries[pdpt_idx] & PAGE_ADDR_MASK);
    struct MappingTable* pt = (struct MappingTable*)(pd->entries[pd_idx] & PAGE_ADDR_MASK);

    if (!(pt->entries[pt_idx] & VMM_P_PRESENT))
        pt->entries[pt_idx] = (logical & PAGE_ADDR_MASK);

    // Evict the page from the Translation Look Aside Buffer. Bye bye! >:)
    __asm__ __volatile__("invlpg (%0)" :: "r" (logical));
}



void* kalloc_page(uint32_t flags) {
    uint64_t addr = alloc_frame_internal();
    unmap_page(addr);
    map_page(addr, flags);
    return (void*)addr;
}


// Implemented in PML4.asm 
void load_pml4(void* pml4);


void vmm_init(meminfo_t meminfo) {
    memzero(&pml4, PAGE_SIZE);
    log("<VMM>: PML4 zeroed.\n", S_INFO);

    set_placement(meminfo);
    log("<VMM>: Placement set.\n", S_INFO);

    if (placement == 0xDEADBEEF)
        panic("NO MEMORY LEFT!\n");

    for (uint64_t i = 0; i < GB*20; i += PAGE_SIZE) {
        if (i >= KERNEL_RESERVED_START && i <= KERNEL_RESERVED_END) {
            map_page(i, VMM_P_PRESENT | VMM_RW_WRITABLE);
            continue;
        }

        map_page(i, VMM_P_PRESENT | VMM_RW_WRITABLE | VMM_US_USER);
    }

    log("<VMM>: Kernel address space has been mapped.\n", S_INFO);
    load_pml4(&pml4);
    log("<VMM>: CR3 loaded with page aligned PML4 base.\n", S_INFO);
}
