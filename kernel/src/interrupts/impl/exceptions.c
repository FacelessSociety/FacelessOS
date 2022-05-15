#include <interrupts/exceptions.h>

// 2022 Ian Moffett <ian@kesscoin.com>

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
