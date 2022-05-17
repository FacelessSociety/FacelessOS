;; 2022 Ian Moffett <ian@kesscoin.com>
bits 64

global libasm_get_dateyear
global libasm_get_datemonth
global libasm_get_dateday

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
