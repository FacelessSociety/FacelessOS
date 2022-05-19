global libgfx_draw_square

libgfx_draw_square:
    mov rax, 0x6
    mov r15, rdi
    mov r14, rsi
    mov r13, rdx
    mov r12, rcx
    mov r11, r8
    int 0x80
    retq
