#include <drivers/ps2/Keyboard.h>
#include <arch/io/io.h>
#include <arch/io/legacypic.h>
#include <stddef.h>
#include <protection/r3kc.h>


// 2022 Ian Moffett <ian@kesscoin.com>

static void* ui_routine = NULL;


__attribute__((interrupt)) void ps2_irq1_response(struct InterruptStackFrame*) {
    uint32_t scancode = inportb(0x60);

    if (scancode & 0x80) {
        PIC_sendEOI(1);
        return;
    }

    if (ui_routine != NULL) {
        r3kc_exec_callback(ui_routine, scancode);
    }


    PIC_sendEOI(1);
    inportb(0x60);
}


void ps2_keyb_hook_ui(void* routine) {
    if (ui_routine != NULL) return;
    ui_routine = routine;
}
