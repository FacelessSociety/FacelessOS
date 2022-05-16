;; 2022 Ian Moffett <ian@kesscoin.com>
bits 64

global syscall_entry

extern dispatch_syscall

syscall_entry:
    mov [regs + 0 * 8], r15
    mov [regs + 1 * 8], r14
    mov [regs + 2 * 8], r13
    mov [regs + 3 * 8], r12
    mov [regs + 4 * 8], r11
    mov [regs + 5 * 8], r10

    mov rdi, rax
    mov rsi, regs

    call dispatch_syscall
    iretq


regs: dq 0, 0, 0, 0, 0, 0
