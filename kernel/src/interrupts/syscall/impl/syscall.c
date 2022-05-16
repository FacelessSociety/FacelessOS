#include <stdint.h>
#include <debug/log.h>
#include <drivers/ps2/Keyboard.h>

#define SYSCALL_COUNT 1

static volatile struct SyscallRegs {
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
} __attribute__((packed)) *regs;


static void sys_test(void) {
    log("Hello Syscall!\n", S_INFO);
}


// An array of syscall functions.
static void(*syscall_table[SYSCALL_COUNT])(void) = {
    sys_test,
};



void dispatch_syscall(uint64_t rax, struct SyscallRegs* registers) {
    if (rax >= SYSCALL_COUNT) return;
    regs = registers;
    syscall_table[rax]();
}
