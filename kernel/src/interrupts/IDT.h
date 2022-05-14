#ifndef IDT_H
#define IDT_H

// 2022 Ian Moffett <ian@kesscoin.com>


#include <stdint.h>

#define TRAP_GATE_FLAGS 0x8F
#define INT_GATE_FLAGS 0x8E
#define IDT_INT_GATE_USER 0xEE


struct IDTGateDescriptor {
    uint16_t isr_low16;
    uint16_t cs;
    uint8_t ist : 2;
    uint8_t zero : 1;
    uint8_t zero1 : 3;
    uint8_t type : 4;
    uint8_t zero2 : 1;
    uint8_t dpl : 2;
    uint8_t p : 1;
    uint16_t isr_mid16;
    uint32_t isr_high32;
    uint32_t reserved;
};


struct __attribute__((packed)) IDTR {
    uint16_t limit;
    uint64_t base;
};


struct __attribute__((packed)) InterruptStackFrame {
    uint64_t error_code;
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};


void idt_set_vector(uint16_t vec, void* isr, uint32_t flags);
void idt_install(void);
