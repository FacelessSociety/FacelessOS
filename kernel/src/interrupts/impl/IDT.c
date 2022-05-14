#include <interrupts/IDT.h>

static struct IDTGateDescriptor idt[256];
static struct IDTR idtr;


void idt_set_vector(uint16_t vec, void* isr, uint32_t flags) {
    uint64_t addr = (uint64_t)isr;
    idt[vec].isr_low16 = addr & 0xFFFF;
    idt[vec].isr_mid16 = (addr >> 16) & 0xFFFF;
    idt[vec].isr_high32 = (addr >> 32) & 0xFFFFFFFF;
    idt[vec].cs = 0x08;
    idt[vec].ist = 0;
    idt[vec].zero = 0;
    idt[vec].zero1 = 0;
    idt[vec].zero2 = 0;
    idt[vec].dpl = 3;
    idt[vec].p = 1;
    idt[vec].reserved = 0;
}


void idt_install(void) {
    idtr.limit = sizeof(struct IDTGateDescriptor) * 256 - 1;
    idtr.base = (uint64_t)&idt;
    __asm__ __volatile__("lidt %0" :: "m" (idtr));
}
