;; 2022 Ian Moffett <ian@kesscoin.com>
bits 64

global libasm_get_dateyear
global libasm_get_datemonth
global libasm_get_dateday
global libasm_pcspkr_toggle
global libasm_writech_xy

libasm_get_dateyear:
    mov rax, 0xB
    int 0x80
    mov rax, r15
    retq


libasm_get_datemonth:
    mov rax, 0xA            ;; SYS_GET_DATE.
    int 0x80                ;; SYSCALL.
    mov rax, r15            ;; MONTH.
    retq


libasm_get_dateday:
    mov rax, 0xC
    int 0x80
    mov rax, r15
    retq


libasm_pcspkr_toggle:
    mov rax, 0xD
    mov r15, 500
    int 0x80
    retq

libasm_writech_xy:
    mov rax, 0x10
    mov r15, rdi
    mov r14, rsi
    mov r13, rdx
    int 0x80
    retq
