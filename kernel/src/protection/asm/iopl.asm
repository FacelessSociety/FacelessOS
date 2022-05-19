;; 2022 Ian Moffett <ian@kesscoin.com>
bits 64

global zero_iopl

zero_iopl:
    pushf
    pop rax
    and rax, ~(1 << 12 | 1 << 13)
    push rax
    popf
    retq
