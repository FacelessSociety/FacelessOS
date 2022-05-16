#include <proc/TSS.h>
#include <arch/memory/vmm.h>
#include <util/mem.h>

// 2022 Ian Moffett <ian@kesscoin.com>
extern struct TSSDescriptor* GDT_TSS;
static struct TSSEntry tss;

void load_task_register();


void init_tss() {
    // Userspace needs a stack.
    uint64_t stack = (uint64_t)kalloc_page(VMM_P_PRESENT | VMM_US_USER | VMM_RW_WRITABLE);
    stack += 4080;
    memzero(&tss, sizeof(struct TSSEntry));     // Ensure everything is zero.
    // All we need is to store the stack in the TSS.
    tss.rsp0Low = stack & 0xFFFFFFFF;         
    tss.rsp0High = (stack >> 32) & 0xFFFFFFFF;

    uint64_t tssp = (uint64_t)&tss;                         // TSS pointer in uint64_t form.
    GDT_TSS->seglimit = sizeof(struct TSSDescriptor);       // Seglimit would be size of the descriptor.
    GDT_TSS->baseAddrLow = tssp & 0xFFFF;                   // Low 16 bits of tssp.
    GDT_TSS->baseAddrMiddle = (tssp >> 16) & 0xFF;          // Middle 8 bits of tssp.
    GDT_TSS->baseAddrUpper = (tssp >> 24) & 0xFFFFFFFF;     // Top 32 bits of tssp.
    GDT_TSS->type = 0x9;                                    // Type stays 0x9.
    GDT_TSS->zero = 0;                                      // Keep zero.
    GDT_TSS->dpl = 0;                                       // Only ring 0 programs can access this descriptor.
    GDT_TSS->p = 1;                                         // 1 for present.
    GDT_TSS->g = 1;                                         // Granularity.
    load_task_register();  
}
