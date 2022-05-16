#include <interrupts/IDT.h>
#include <debug/log.h>
#include <stdint.h>

// Page fault bits.
#define PF_PRESENT (1 << 0)
#define PF_WRITE (1 << 1)
#define PF_USER (1 << 2)
#define PF_RESV_WRITE (1 << 3)
#define PF_INST_FETCH (1 << 4)
#define PF_ACCESS_VIOLATION (1 << 5)
#define PF_SHADOW_STACK (1 << 6)
#define LOG(MSG) log(MSG, -1)


void dump_exception_log(struct InterruptStackFrame* stackframe, uint8_t vector) {
    switch (vector) {
        case 0x0:
            LOG("Divide Error.\n");
            break;
        case 0x1:
            LOG("Debug Exception.\n");
            break;
        case 0x4:
            LOG("Overflow.\n");
            break;
        case 0x6:
            LOG("Invalid Opcode.\n");
            break;
        case 0x7:
            LOG("Device Not Available.\n");
            break;
        case 0x8:
            LOG("Double Fault.\n");
            break;
        case 0xA:
            LOG("Invalid Task State Segment.\n");
            break;
        case 0xB:
            LOG("Segment Not Present.\n");
            break;
        case 0xC:
            LOG("Stack Segment Fault.\n");
            break;
        case 0xD:
            if (stackframe->error_code != 0) {
                log("GPF: Segmentation fault.\n", S_CRITICAL);

                if (stackframe->error_code & 1)
                    log("Exception origin external to CPU.\n", -1);
                else
                    log("Exception origin internal to CPU.\n", -1);

                switch ((stackframe->error_code >> 1) & 0b11) {
                    case 0b00:
                        log("Exception occurred within: __KESSOS_INTERNAL_GDT__\n", -1);
                        break;
                    case 0b01:
                    case 0b11:
                        log("Exception occurred within: __KESSOS_INTERNAL__IDT__\n", -1);
                        break;
                    default:
                        log("FATAL: COULD NOT FIND EXCEPTION ORIGIN.\n", S_CRITICAL);
                        return;
                }

                uint16_t desc_tbl_idx = (stackframe->error_code >> 3) & 0x1FFF;
                log("Selector index: %x\n", -1, desc_tbl_idx);
            } else {
                LOG("General Protection Fault.\n");
            }
            break;
        case 0xE:
            LOG("PAGE_FAULT\n\n");
            LOG("-|");

            if (stackframe->error_code & PF_PRESENT)
                LOG("P|");

            if (stackframe->error_code & PF_WRITE)
                LOG("W|");

            if (stackframe->error_code & PF_USER)
                LOG("U|");

            if (stackframe->error_code & PF_RESV_WRITE)
                LOG("R|");

            if (stackframe->error_code & PF_INST_FETCH)
                LOG("I|");

            if (stackframe->error_code & PF_ACCESS_VIOLATION)
                LOG("PK|");

            if (stackframe->error_code & PF_SHADOW_STACK)
                LOG("SS|");

            LOG("-");
    }
}
