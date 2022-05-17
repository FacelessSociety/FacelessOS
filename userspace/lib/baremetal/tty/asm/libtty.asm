;; 2022 Ian Moffett <ian@kesscoin.com>
bits 64
global libvtty_writech

libvtty_writech:
    mov rax, 0x4
    mov r15, rdi
    int 0x80
    retq
