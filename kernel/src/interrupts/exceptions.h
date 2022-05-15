#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

// 2022 Ian Moffett <ian@kesscoin.com>



void divide_error(void);
void debug_exception(void);
void overflow_handler(void);
void br_exception(void);
void invalid_opcode_handler(void);
void dev_not_avl(void);
void double_fault(void);
void invld_tss(void);
void segment_not_present(void);
void stack_segfault(void);
void general_protection(void);
void page_fault(void);

// A nice array of exception handler pointers so
// we can just iterate over them and made things
// nice and clean.
extern void(*exceptions[])(void);


#endif
