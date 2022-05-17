#ifndef PS2_KEYBOARD_DRIVER_H
#define PS2_KEYBOARD_DRIVER_H

// 2022 Ian Moffett <ian@kesscoin.com>

#include <interrupts/IDT.h>


__attribute__((interrupt)) void ps2_irq1_response(struct InterruptStackFrame*);
uint16_t ps2_keyb_fetch_scancode(void);
uint8_t ps2_keyb_fetch_status(void);

#endif
