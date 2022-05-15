#include <interrupts/exceptions.h>
#include <drivers/video/FrameBuffer.h>
#include <debug/log.h>

// 2022 Ian Moffett <ian@kesscoin.com>

#define PANIC_BG_COLOR 0x00008B


void(*exceptions[])(void) = {
    divide_error,
    debug_exception,
    overflow_handler,
    br_exception,
    invalid_opcode_handler,
    dev_not_avl,
    double_fault,
    invld_tss,
    segment_not_present,
    stack_segfault,
    general_protection,
    page_fault
};


void panic(const char* panic_msg) {
    extern canvas_t canvas;
    clearScreen(&canvas, PANIC_BG_COLOR);
    log(panic_msg, S_PANIC);
    __asm__ __volatile__("cli; hlt");
}
