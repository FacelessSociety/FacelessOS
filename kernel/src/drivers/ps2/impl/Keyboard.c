#include <drivers/ps2/Keyboard.h>
#include <debug/log.h>
#include <arch/io/io.h>
#include <arch/io/legacypic.h>
#include <stddef.h>
#include <protection/r3kc.h>

#define KEYB_IN_ISDIGIT(C) (C >= '0' && C <= '9')
#define KEYB_IN_ALPHA(C) (C >= 'a' && C <= 'z')


// 2022 Ian Moffett <ian@kesscoin.com>

static uint16_t scancode;
static uint8_t keyboard_status = 0;


__attribute__((interrupt)) void ps2_irq1_response(struct InterruptStackFrame*) {
    scancode = inportb(0x60);

    if (scancode & 0x80) {
        keyboard_status &= ~(1 << 1);       // Unsetting pressed  bit.
        PIC_sendEOI(1);
        return;
    }

    keyboard_status |= (1 << 0);            // Setting IRQ_FIRED bit.
    keyboard_status |= (1 << 1);            // Setting PRESSED bit.

    PIC_sendEOI(1);
    inportb(0x60);
}


uint16_t ps2_keyb_fetch_scancode(void) {
    return scancode;
}


uint8_t ps2_keyb_fetch_status(void) {
    return keyboard_status;
}
