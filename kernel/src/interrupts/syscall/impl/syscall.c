#include <stdint.h>
#include <debug/log.h>
#include <drivers/ps2/Keyboard.h>
#include <util/asm.h>


#define SYSCALL_COUNT 5

static volatile struct SyscallRegs {
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
} __attribute__((packed)) *regs;

extern uint64_t* syscall_return;


static void sys_test(void) {
    log("Hello Syscall!\n", S_INFO);
}


// Returns keyboard status in R15.
static void sys_keyboard_status(void) {
    regs->r15 = ps2_keyb_fetch_status();
}


// Returns keyboard scancode in R15.
static void sys_keyb_scancode(void) {
    regs->r15 = ps2_keyb_fetch_scancode();
}


// Writes a char onto screen while running in baremetal mode.
static void sys_baremetal_writech(void) {
    CLI;
    char terminated_char[2] = {regs->r15, 0x0};
    log("%s", -1, terminated_char);
    STI;
}


// Waits for interrupt.
static void sys_int_wait(void) {
    STI;
    HLT;
}


// An array of syscall functions.
static void(*syscall_table[SYSCALL_COUNT])(void) = {
    sys_test,
    sys_keyboard_status,
    sys_keyb_scancode,
    sys_int_wait,
    sys_baremetal_writech
};



void dispatch_syscall(uint64_t rax, struct SyscallRegs* registers) {
    if (rax >= SYSCALL_COUNT) return;
    regs = registers;
    syscall_table[rax]();
}
